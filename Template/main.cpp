#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Models.h"
#include "GameLogic.h"
#include "UIAndCutscene.h"

int g_screenWidth = 1000;
int g_screenHeight = 700;

const char* vertexShaderSource = R"(
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 vColor;
uniform mat4 u_Transform;
void main()
{
    gl_Position = u_Transform * vec4(aPos, 1.0);
    vColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 430 core
in vec3 vColor;
out vec4 FragColor;
uniform float u_Highlight;
uniform float u_RedTint;
uniform int   u_Grayscale;
uniform vec3  u_OverrideColor;
uniform int   u_UseOverride;
void main()
{
    vec3 baseCol = (u_UseOverride == 1) ? u_OverrideColor : vColor;
    if (u_Grayscale == 1) {
        float gray = dot(baseCol, vec3(0.299, 0.587, 0.114));
        baseCol = vec3(gray * 0.7);
    }
    vec3 damagedCol = mix(baseCol, vec3(0.95, 0.1, 0.1), u_RedTint);
    vec3 finalColor = mix(damagedCol, vec3(1.0, 1.0, 0.45), u_Highlight);
    FragColor = vec4(finalColor, 1.0);
}
)";

Mesh g_quadMesh;
Mesh g_triangleMesh;
Mesh g_circleMesh;

GLuint g_shaderProgram = 0;
GLint locTransform = -1;
GLint locHighlight = -1;
GLint locRedTint = -1;
GLint locGrayscale = -1;
GLint locOverrideColor = -1;
GLint locUseOverride = -1;

GameState g_gameState = STATE_MAIN_MENU;
float g_cutsceneTimer = 0.0f;

const int NUM_LANES = 4;
const float LANE_BOTTOM_Y[NUM_LANES] = { 0.20f, -0.15f, -0.50f, -0.85f };
const float LANE_TOP_Y[NUM_LANES] = { 0.55f,  0.20f, -0.15f, -0.50f };
const float LANE_CENTER_Y[NUM_LANES] = { 0.375f, 0.025f, -0.325f, -0.675f };
const float LANE_LEFT_X = -0.78f;
const float LANE_RIGHT_X = 0.95f;

bool g_laneSelected[NUM_LANES] = { false, false, false, false };
int  g_hoveredLane = -1;

const int PLAYER_COST[3] = { 10, 20, 30 };
int  g_coins = 30;
bool g_hasEarnedExtraCoins = false;
int  g_selectedCard = 0;
int  g_hoveredCard = -1;

std::vector<PlayerUnit>  g_players;
std::vector<EnemyUnit>   g_enemies;
std::vector<Projectile>  g_projectiles;
std::vector<Collectible> g_collectibles;

int   g_currentWave = 1;
int   g_enemiesToSpawn = 0;
float g_spawnTimer = 0.0f;

glm::vec2 g_cursorNDC(0.0f, 0.0f);

inline void startWave(int wave)
{
    g_currentWave = wave;
    g_enemiesToSpawn = wave * 3 + 2;
    g_spawnTimer = 0.0f;
}

inline void resetGame()
{
    g_players.clear();
    g_enemies.clear();
    g_projectiles.clear();
    g_collectibles.clear();
    g_coins = 30;
    g_hasEarnedExtraCoins = false;
    for (int i = 0; i < NUM_LANES; ++i) {
        g_laneSelected[i] = false;
    }
    startWave(1);
}
void updateGameplay(float dt)
{
    if (g_enemiesToSpawn > 0) {
        g_spawnTimer -= dt;
        if (g_spawnTimer <= 0.0f) {
            spawnEnemyForCurrentWave(g_currentWave, g_enemies, LANE_RIGHT_X);
            g_enemiesToSpawn--;
            g_spawnTimer = 3.2f;
        }
    }
    else {
        bool anyEnemyAlive = false;
        for (int i = 0; i < (int)g_enemies.size(); ++i) {
            if (g_enemies[i].alive) { anyEnemyAlive = true; break; }
        }
        if (!anyEnemyAlive) {
            if (g_currentWave < 3) {
                startWave(g_currentWave + 1);
            }
            else {
                g_gameState = STATE_WIN;
                return;
            }
        }
    }

    for (int i = 0; i < (int)g_players.size(); ++i) g_players[i].inMeleeCombat = false;
    for (int i = 0; i < (int)g_enemies.size(); ++i) g_enemies[i].inMeleeCombat = false;

    for (int i = 0; i < (int)g_players.size(); ++i) {
        if (!g_players[i].alive) continue;
        for (int j = 0; j < (int)g_enemies.size(); ++j) {
            if (!g_enemies[j].alive || g_enemies[j].lane != g_players[i].lane) continue;
            if (std::abs(g_players[i].x - g_enemies[j].x) <= 0.085f) {
                g_players[i].inMeleeCombat = true;
                g_enemies[j].inMeleeCombat = true;
            }
        }
    }

    for (int i = 0; i < (int)g_players.size(); ++i) {
        PlayerUnit& p = g_players[i];
        if (!p.alive) continue;

        if (p.isJumping || p.isDucking) {
            p.actionTimer -= dt;
            if (p.actionTimer <= 0.0f) {
                p.isJumping = false;
                p.isDucking = false;
            }
        }

        if (p.type != PLAYER_DEFENSE && !p.inMeleeCombat) {
            p.x += 0.14f * dt;
            if (p.x > LANE_RIGHT_X) p.x = LANE_RIGHT_X;
        }

        p.attackTimer -= dt;
        if (p.attackTimer <= 0.0f) {
            if (p.type == PLAYER_RANGED) {
                Projectile proj;
                proj.lane = p.lane;
                proj.x = p.x + 0.04f;
                proj.fromPlayer = true;
                proj.isHigh = false;
                proj.damage = 1;
                proj.hitting = false;
                proj.hitAnimTimer = 0.0f;
                proj.alive = true;
                g_projectiles.push_back(proj);
                p.attackTimer = 1.4f;
            }
            else if (p.type == PLAYER_MELEE) {
                for (int j = 0; j < (int)g_enemies.size(); ++j) {
                    EnemyUnit& e = g_enemies[j];
                    if (e.alive && e.lane == p.lane && std::abs(p.x - e.x) <= 0.10f) {
                        e.hp -= 1;
                        if (e.hp <= 0) {
                            e.alive = false;
                            Collectible c;
                            c.lane = e.lane;
                            c.x = e.x;
                            c.value = 10;
                            c.alive = true;
                            g_collectibles.push_back(c);
                        }
                        break;
                    }
                }
                p.attackTimer = 1.0f;
            }
        }

        for (int j = 0; j < (int)g_collectibles.size(); ++j) {
            Collectible& c = g_collectibles[j];
            if (c.alive && c.lane == p.lane && std::abs(p.x - c.x) <= 0.075f) {
                c.alive = false;
                g_coins += c.value;
                g_hasEarnedExtraCoins = true;
            }
        }
    }

    for (int i = 0; i < (int)g_enemies.size(); ++i) {
        EnemyUnit& e = g_enemies[i];
        if (!e.alive) continue;

        float speed = (e.type == ENEMY_HARD_MELEE) ? 0.19f : 0.10f;
        if (!e.inMeleeCombat) {
            e.x -= speed * dt;
        }

        if (e.x <= LANE_LEFT_X) {
            g_gameState = STATE_LOSE;
            return;
        }

        e.attackTimer -= dt;
        if (e.attackTimer <= 0.0f) {
            if (e.type == ENEMY_EASY_UPPER || e.type == ENEMY_MEDIUM_LOWER) {
                Projectile proj;
                proj.lane = e.lane;
                proj.x = e.x - 0.04f;
                proj.fromPlayer = false;
                proj.isHigh = (e.type == ENEMY_EASY_UPPER);
                proj.damage = 1;
                proj.hitting = false;
                proj.hitAnimTimer = 0.0f;
                proj.alive = true;
                g_projectiles.push_back(proj);
                e.attackTimer = 1.8f;
            }
            else if (e.type == ENEMY_HARD_MELEE) {
                for (int j = 0; j < (int)g_players.size(); ++j) {
                    PlayerUnit& p = g_players[j];
                    if (p.alive && p.lane == e.lane && std::abs(p.x - e.x) <= 0.10f) {
                        p.hp -= 2;
                        if (p.hp <= 0) p.alive = false;
                        break;
                    }
                }
                e.attackTimer = 1.1f;
            }
        }
    }

    for (int i = 0; i < (int)g_projectiles.size(); ++i) {
        Projectile& proj = g_projectiles[i];
        if (!proj.alive) continue;

        if (proj.hitting) {
            proj.hitAnimTimer -= dt;
            if (proj.hitAnimTimer <= 0.0f) proj.alive = false;
            continue;
        }

        proj.x += (proj.fromPlayer ? 0.55f : -0.45f) * dt;
        if (proj.x < LANE_LEFT_X || proj.x > LANE_RIGHT_X + 0.05f) {
            proj.alive = false;
            continue;
        }

        if (proj.fromPlayer) {
            for (int j = 0; j < (int)g_enemies.size(); ++j) {
                EnemyUnit& e = g_enemies[j];
                if (e.alive && e.lane == proj.lane && std::abs(proj.x - e.x) <= 0.045f) {
                    e.hp -= proj.damage;
                    proj.hitting = true;
                    proj.hitAnimTimer = 0.20f;
                    if (e.hp <= 0) {
                        e.alive = false;
                        Collectible c;
                        c.lane = e.lane;
                        c.x = e.x;
                        c.value = 10;
                        c.alive = true;
                        g_collectibles.push_back(c);
                    }
                    break;
                }
            }
        }
        else {
            float defenseX = -999.0f;
            for (int j = 0; j < (int)g_players.size(); ++j) {
                if (g_players[j].alive && g_players[j].lane == proj.lane && g_players[j].type == PLAYER_DEFENSE) {
                    if (g_players[j].x > defenseX) defenseX = g_players[j].x;
                }
            }

            for (int j = 0; j < (int)g_players.size(); ++j) {
                PlayerUnit& p = g_players[j];
                if (!p.alive || p.lane != proj.lane) continue;

                if (p.type != PLAYER_DEFENSE && p.x < defenseX) continue;

                if (std::abs(proj.x - p.x) <= 0.045f) {
                    if (p.type != PLAYER_DEFENSE) {
                        if (proj.isHigh && p.isDucking) continue;
                        if (!proj.isHigh && p.isJumping) continue;
                    }
                    p.hp -= proj.damage;
                    proj.alive = false;
                    if (p.hp <= 0) p.alive = false;
                    break;
                }
            }
        }
    }
}

void framebufferSizeCallback(GLFWwindow*, int width, int height)
{
    g_screenWidth = width;
    g_screenHeight = height;
    glViewport(0, 0, width, height);
}

void cursorPosCallback(GLFWwindow*, double xpos, double ypos)
{
    g_cursorNDC.x = (2.0f * (float)xpos) / (float)g_screenWidth - 1.0f;
    g_cursorNDC.y = 1.0f - (2.0f * (float)ypos) / (float)g_screenHeight;

    g_hoveredLane = -1;
    if (g_cursorNDC.x >= LANE_LEFT_X && g_cursorNDC.x <= LANE_RIGHT_X) {
        for (int i = 0; i < NUM_LANES; ++i) {
            if (g_cursorNDC.y >= LANE_BOTTOM_Y[i] && g_cursorNDC.y <= LANE_TOP_Y[i]) {
                g_hoveredLane = i;
                break;
            }
        }
    }

    g_hoveredCard = -1;
    for (int i = 0; i < 3; ++i) {
        glm::vec2 cardCenter(-0.68f + i * 0.24f, 0.87f);
        if (isCardUnlocked(i, g_coins, g_hasEarnedExtraCoins) && isPointInBox(g_cursorNDC, cardCenter, glm::vec2(0.20f, 0.21f))) {
            g_hoveredCard = i;
            break;
        }
    }
}

void spawnPlayerInLane(int lane, PlayerType type)
{
    int cost = PLAYER_COST[(int)type];
    if (!isCardUnlocked((int)type, g_coins, g_hasEarnedExtraCoins) || g_coins < cost) return;

    g_coins -= cost;

    PlayerUnit p;
    p.type = type;
    p.lane = lane;
    p.x = (type == PLAYER_DEFENSE) ? -0.35f : LANE_LEFT_X + 0.04f;
    p.maxHp = (type == PLAYER_DEFENSE) ? 7 : 5;
    p.hp = p.maxHp;
    p.attackTimer = 0.2f;
    p.isJumping = false;
    p.isDucking = false;
    p.actionTimer = 0.0f;
    p.inMeleeCombat = false;
    p.alive = true;
    g_players.push_back(p);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS) return;

    if (g_gameState == STATE_MAIN_MENU) {
        if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, 0.15f), glm::vec2(0.45f, 0.14f))) {
            g_cutsceneTimer = 0.0f;
            g_gameState = STATE_CUTSCENE;
        }
        else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, -0.15f), glm::vec2(0.45f, 0.14f))) {
            glfwSetWindowShouldClose(window, true);
        }
        return;
    }

    if (g_gameState == STATE_PAUSED) {
        if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, 0.22f), glm::vec2(0.45f, 0.13f))) {
            g_gameState = STATE_PLAYING;
        }
        else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, 0.0f), glm::vec2(0.45f, 0.13f))) {
            resetGame();
            g_gameState = STATE_PLAYING;
        }
        else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, -0.22f), glm::vec2(0.45f, 0.13f))) {
            g_gameState = STATE_MAIN_MENU;
        }
        return;
    }

    if (g_gameState == STATE_WIN || g_gameState == STATE_LOSE) {
        if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, 0.08f), glm::vec2(0.45f, 0.14f))) {
            resetGame();
            g_gameState = STATE_PLAYING;
        }
        else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, -0.16f), glm::vec2(0.45f, 0.14f))) {
            g_gameState = STATE_MAIN_MENU;
        }
        return;
    }

    if (g_gameState == STATE_PLAYING) {
        if (g_hoveredCard != -1) {
            g_selectedCard = g_hoveredCard;
            for (int i = 0; i < NUM_LANES; ++i) {
                if (g_laneSelected[i]) {
                    spawnPlayerInLane(i, (PlayerType)g_selectedCard);
                    break;
                }
            }
            return;
        }

        if (g_hoveredLane != -1) {
            g_laneSelected[g_hoveredLane] = !g_laneSelected[g_hoveredLane];
        }
    }
}

void keyCallback(GLFWwindow*, int key, int, int action, int)
{
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) {
        if (g_gameState == STATE_PLAYING) g_gameState = STATE_PAUSED;
        else if (g_gameState == STATE_PAUSED) g_gameState = STATE_PLAYING;
        return;
    }

    if (g_gameState != STATE_PLAYING) return;

    if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
        for (int i = 0; i < (int)g_players.size(); ++i) {
            PlayerUnit& p = g_players[i];
            if (p.alive && p.type != PLAYER_DEFENSE && g_laneSelected[p.lane]) {
                p.isJumping = true;
                p.isDucking = false;
                p.actionTimer = 0.65f;
            }
        }
    }
    else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
        for (int i = 0; i < (int)g_players.size(); ++i) {
            PlayerUnit& p = g_players[i];
            if (p.alive && p.type != PLAYER_DEFENSE && g_laneSelected[p.lane]) {
                p.isDucking = true;
                p.isJumping = false;
                p.actionTimer = 0.65f;
            }
        }
    }
    else if (key == GLFW_KEY_SPACE || key == GLFW_KEY_ENTER) {
        for (int i = 0; i < NUM_LANES; ++i) {
            if (g_laneSelected[i]) {
                spawnPlayerInLane(i, (PlayerType)g_selectedCard);
            }
        }
    }
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(g_screenWidth, g_screenHeight, "Castle Siege - 2D Tower Defense", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }

    glViewport(0, 0, g_screenWidth, g_screenHeight);

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    g_shaderProgram = glCreateProgram();
    glAttachShader(g_shaderProgram, vertexShader);
    glAttachShader(g_shaderProgram, fragmentShader);
    glLinkProgram(g_shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    locTransform = glGetUniformLocation(g_shaderProgram, "u_Transform");
    locHighlight = glGetUniformLocation(g_shaderProgram, "u_Highlight");
    locRedTint = glGetUniformLocation(g_shaderProgram, "u_RedTint");
    locGrayscale = glGetUniformLocation(g_shaderProgram, "u_Grayscale");
    locOverrideColor = glGetUniformLocation(g_shaderProgram, "u_OverrideColor");
    locUseOverride = glGetUniformLocation(g_shaderProgram, "u_UseOverride");

    const float quadVertices[] = {
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f
    };
    g_quadMesh = createMesh(quadVertices, sizeof(quadVertices), GL_TRIANGLES);

    const float triVertices[] = {
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.0f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f
    };
    g_triangleMesh = createMesh(triVertices, sizeof(triVertices), GL_TRIANGLES);

    const int segments = 50;
    std::vector<float> circleVerts;
    circleVerts.insert(circleVerts.end(), { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f });
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159265f * (float)i / (float)segments;
        circleVerts.insert(circleVerts.end(), {
            std::cos(angle), std::sin(angle), 0.0f,
            1.0f, 1.0f, 1.0f
            });
    }
    g_circleMesh = createMesh(circleVerts.data(), circleVerts.size() * sizeof(float), GL_TRIANGLE_FAN);

    float lastFrame = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.12f, 0.14f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(g_shaderProgram);
        glm::mat4 base(1.0f);

        if (g_gameState == STATE_MAIN_MENU) {
            drawEnvironment(currentFrame);
            drawHomeBase();
            drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.0f), glm::vec2(0.75f, 0.75f), 0.0f, glm::vec3(0.14f, 0.16f, 0.22f));
            drawButton(glm::vec2(0.0f, 0.15f), glm::vec2(0.45f, 0.14f), glm::vec3(0.20f, 0.65f, 0.30f), 1, g_cursorNDC);
            drawButton(glm::vec2(0.0f, -0.15f), glm::vec2(0.45f, 0.14f), glm::vec3(0.75f, 0.22f, 0.22f), 3, g_cursorNDC);
        }
        else if (g_gameState == STATE_CUTSCENE) {
            renderCutscene(currentFrame, g_cutsceneTimer);
            g_cutsceneTimer += deltaTime;
            if (g_cutsceneTimer >= 14.0f) {
                resetGame();
                g_gameState = STATE_PLAYING;
            }
        }
        else {
            if (g_gameState == STATE_PLAYING) {
                updateGameplay(deltaTime);
            }

            drawEnvironment(currentFrame);
            renderTopBarUI(currentFrame, g_coins, g_hasEarnedExtraCoins, g_selectedCard, g_hoveredCard, g_currentWave);

            for (int i = 0; i < NUM_LANES; ++i) {
                float cy = LANE_CENTER_Y[i];
                float h = LANE_TOP_Y[i] - LANE_BOTTOM_Y[i];
                glm::vec3 laneCol = (i % 2 == 0) ? glm::vec3(0.32f, 0.65f, 0.24f) : glm::vec3(0.27f, 0.58f, 0.20f);

                float highlight = 0.0f;
                if (g_laneSelected[i]) highlight = 0.38f;
                else if (g_hoveredLane == i && g_gameState == STATE_PLAYING) highlight = 0.22f;

                drawPart(g_quadMesh, base, glm::vec2(0.085f, cy), glm::vec2(1.73f, h - 0.01f), 0.0f, laneCol, 0.0f, highlight);
                drawPart(g_quadMesh, base, glm::vec2(0.085f, LANE_BOTTOM_Y[i]), glm::vec2(1.73f, 0.008f), 0.0f, glm::vec3(0.18f, 0.38f, 0.12f));
            }

            drawHomeBase();

            for (int i = 0; i < (int)g_collectibles.size(); ++i) {
                if (g_collectibles[i].alive) drawCollectibleCoin(glm::vec2(g_collectibles[i].x, LANE_CENTER_Y[g_collectibles[i].lane] - 0.04f), currentFrame);
            }

            for (int i = 0; i < (int)g_players.size(); ++i) {
                const PlayerUnit& p = g_players[i];
                if (!p.alive) continue;
                float redIntensity = (1.0f - (float)p.hp / (float)p.maxHp) * 0.85f;
                float animTime = (g_gameState == STATE_PLAYING) ? currentFrame : 0.0f;
                glm::vec2 pos(p.x, LANE_CENTER_Y[p.lane]);
                drawPlayerModel(p.type, pos, animTime, redIntensity, p.isJumping, p.isDucking);
                drawHealthBar(pos + glm::vec2(0.0f, 0.135f), p.hp, p.maxHp);
            }

            for (int i = 0; i < (int)g_enemies.size(); ++i) {
                const EnemyUnit& e = g_enemies[i];
                if (!e.alive) continue;
                float redIntensity = (1.0f - (float)e.hp / (float)e.maxHp) * 0.85f;
                float animTime = (g_gameState == STATE_PLAYING) ? currentFrame : 0.0f;
                glm::vec2 pos(e.x, LANE_CENTER_Y[e.lane]);
                drawEnemyModel(e.type, pos, animTime, redIntensity);
                drawHealthBar(pos + glm::vec2(0.0f, 0.135f), e.hp, e.maxHp);
            }

            for (int i = 0; i < (int)g_projectiles.size(); ++i) {
                if (g_projectiles[i].alive) drawProjectile(g_projectiles[i], currentFrame);
            }

            if (g_gameState == STATE_PAUSED) {
                drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.0f), glm::vec2(0.65f, 0.78f), 0.0f, glm::vec3(0.12f, 0.14f, 0.20f));
                drawButton(glm::vec2(0.0f, 0.22f), glm::vec2(0.45f, 0.13f), glm::vec3(0.20f, 0.65f, 0.30f), 1, g_cursorNDC);
                drawButton(glm::vec2(0.0f, 0.00f), glm::vec2(0.45f, 0.13f), glm::vec3(0.85f, 0.55f, 0.15f), 2, g_cursorNDC);
                drawButton(glm::vec2(0.0f, -0.22f), glm::vec2(0.45f, 0.13f), glm::vec3(0.75f, 0.22f, 0.22f), 3, g_cursorNDC);
            }
            else if (g_gameState == STATE_WIN || g_gameState == STATE_LOSE) {
                glm::vec3 bannerCol = (g_gameState == STATE_WIN) ? glm::vec3(0.15f, 0.55f, 0.25f) : glm::vec3(0.60f, 0.15f, 0.15f);
                drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.0f), glm::vec2(0.70f, 0.68f), 0.0f, bannerCol);
                drawButton(glm::vec2(0.0f, 0.08f), glm::vec2(0.45f, 0.14f), glm::vec3(0.85f, 0.55f, 0.15f), 2, g_cursorNDC);
                drawButton(glm::vec2(0.0f, -0.16f), glm::vec2(0.45f, 0.14f), glm::vec3(0.25f, 0.30f, 0.40f), 3, g_cursorNDC);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyMesh(g_quadMesh);
    destroyMesh(g_triangleMesh);
    destroyMesh(g_circleMesh);
    glDeleteProgram(g_shaderProgram);

    glfwTerminate();
    return 0;
}