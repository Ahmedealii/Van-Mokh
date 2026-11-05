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

bool g_laneSelected[NUM_LANES] = { false, false, false, false };
int  g_hoveredLane = -1;
int  g_lastSelectedLane = -1;

int  g_coins = 30;
bool g_hasEarnedExtraCoins = false;
int  g_selectedCard = 0;
int  g_hoveredCard = -1;
bool g_isPlacingUnit = false;

std::vector<PlayerUnit>  g_players;
std::vector<EnemyUnit>   g_enemies;
std::vector<Projectile>  g_projectiles;
std::vector<Collectible> g_collectibles;

int   g_currentWave = 1;
int   g_enemiesToSpawn = 0;
float g_spawnTimer = 0.0f;

glm::vec2 g_cursorNDC(0.0f, 0.0f);

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
        glm::vec2 cardCenter(-0.68f + (float)i * 0.24f, 0.87f);
        if (isCardUnlocked(i, g_coins, g_hasEarnedExtraCoins) && isPointInBox(g_cursorNDC, cardCenter, glm::vec2(0.20f, 0.21f))) {
            g_hoveredCard = i;
            break;
        }
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int)
{
    if (g_gameState == STATE_MAIN_MENU) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, 0.00f), glm::vec2(0.48f, 0.135f))) {
                g_cutsceneTimer = 0.0f;
                g_gameState = STATE_CUTSCENE;
            }
            else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, -0.17f), glm::vec2(0.42f, 0.120f))) {
                glfwSetWindowShouldClose(window, true);
            }
        }
        return;
    }

    if (g_gameState == STATE_PAUSED) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, 0.06f), glm::vec2(0.46f, 0.120f))) {
                g_gameState = STATE_PLAYING;
            }
            else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, -0.09f), glm::vec2(0.46f, 0.120f))) {
                resetGame();
                g_gameState = STATE_PLAYING;
            }
            else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, -0.24f), glm::vec2(0.46f, 0.120f))) {
                g_gameState = STATE_MAIN_MENU;
            }
        }
        return;
    }

    if (g_gameState == STATE_WIN || g_gameState == STATE_LOSE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, 0.00f), glm::vec2(0.46f, 0.130f))) {
                resetGame();
                g_gameState = STATE_PLAYING;
            }
            else if (isPointInBox(g_cursorNDC, glm::vec2(0.0f, -0.17f), glm::vec2(0.46f, 0.120f))) {
                g_gameState = STATE_MAIN_MENU;
            }
        }
        return;
    }

    if (g_gameState == STATE_PLAYING) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            if (g_isPlacingUnit) {
                g_isPlacingUnit = false;
            }
            else {
                clearAllLanes();
            }
            return;
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (g_hoveredCard != -1) {
                if (isCardUnlocked(g_hoveredCard, g_coins, g_hasEarnedExtraCoins)) {
                    g_selectedCard = g_hoveredCard;
                    g_isPlacingUnit = true;
                }
                return;
            }

            if (g_hoveredLane != -1) {
                if (g_isPlacingUnit) {
                    int cost = PLAYER_COST[g_selectedCard];
                    if (g_coins >= cost && isCardUnlocked(g_selectedCard, g_coins, g_hasEarnedExtraCoins)) {
                        spawnPlayerInLane(g_hoveredLane, (PlayerType)g_selectedCard, g_cursorNDC.x);
                        g_laneSelected[g_hoveredLane] = true;
                        g_lastSelectedLane = g_hoveredLane;
                        g_isPlacingUnit = false;
                    }
                }
                else {
                    toggleLaneSelection(g_hoveredLane);
                }
                return;
            }
        }
    }
}

void keyCallback(GLFWwindow*, int key, int, int action, int)
{
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_ESCAPE) {
        if (g_gameState == STATE_PLAYING) {
            if (g_isPlacingUnit) {
                g_isPlacingUnit = false;
                return;
            }
            g_gameState = STATE_PAUSED;
        }
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
    else if (key == GLFW_KEY_1) {
        if (isCardUnlocked(0, g_coins, g_hasEarnedExtraCoins)) {
            g_selectedCard = 0;
            g_isPlacingUnit = true;
        }
    }
    else if (key == GLFW_KEY_2) {
        if (isCardUnlocked(1, g_coins, g_hasEarnedExtraCoins)) {
            g_selectedCard = 1;
            g_isPlacingUnit = true;
        }
    }
    else if (key == GLFW_KEY_3) {
        if (isCardUnlocked(2, g_coins, g_hasEarnedExtraCoins)) {
            g_selectedCard = 2;
            g_isPlacingUnit = true;
        }
    }
    else if (key == GLFW_KEY_C) {
        clearAllLanes();
    }
    else if (key == GLFW_KEY_SPACE || key == GLFW_KEY_ENTER) {
        deploySelectedUnit();
        g_isPlacingUnit = false;
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

    int linkSuccess = 0;
    glGetProgramiv(g_shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        char infoLog[512];
        glGetProgramInfoLog(g_shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program link error:\n" << infoLog << std::endl;
    }

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

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime > 0.05f) deltaTime = 0.05f;

        glClearColor(0.12f, 0.14f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(g_shaderProgram);
        glm::mat4 base(1.0f);

        if (g_gameState == STATE_MAIN_MENU) {
            renderMainMenuUI(currentFrame, g_cursorNDC);
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

            drawDetailedEnvironment(currentFrame);
            renderTopBarUI(currentFrame, g_coins, g_hasEarnedExtraCoins, g_selectedCard, g_hoveredCard, g_currentWave);

            for (int i = 0; i < NUM_LANES; ++i) {
                float cy = LANE_CENTER_Y[i];
                float h = LANE_TOP_Y[i] - LANE_BOTTOM_Y[i];
                glm::vec3 laneCol = (i % 2 == 0) ? glm::vec3(0.32f, 0.65f, 0.24f) : glm::vec3(0.27f, 0.58f, 0.20f);

                float highlight = 0.0f;
                if (g_isPlacingUnit && g_hoveredLane == i) {
                    highlight = 0.50f;
                }
                else if (g_laneSelected[i]) {
                    highlight = (i == g_lastSelectedLane) ? 0.38f : 0.26f;
                }
                else if (g_hoveredLane == i && g_gameState == STATE_PLAYING) {
                    highlight = 0.16f;
                }

                drawPart(g_quadMesh, base, glm::vec2(0.085f, cy), glm::vec2(1.73f, h - 0.01f), 0.0f, laneCol, 0.0f, highlight);
                drawPart(g_quadMesh, base, glm::vec2(0.085f, LANE_BOTTOM_Y[i]), glm::vec2(1.73f, 0.008f), 0.0f, glm::vec3(0.18f, 0.38f, 0.12f));
            }

            drawDetailedHomeBase();

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
                drawHealthBar(pos + glm::vec2(0.0f, 0.135f), e.hp, e.maxHp); // <-- Change p.maxHp to e.maxHp here
            }

            for (int i = 0; i < (int)g_projectiles.size(); ++i) {
                if (g_projectiles[i].alive) drawProjectile(g_projectiles[i], currentFrame);
            }

            if (g_gameState == STATE_PAUSED) {
                renderPauseMenuUI(currentFrame, g_cursorNDC);
            }
            else if (g_gameState == STATE_WIN || g_gameState == STATE_LOSE) {
                renderEndMenuUI(g_gameState == STATE_WIN, currentFrame, g_cursorNDC);
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