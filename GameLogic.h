#include "Audio.h"
#pragma once
#include <vector>
#include <cmath>
#include <cstdlib>
#include "Models.h"

struct PlayerUnit {
    PlayerType type;
    int   lane;
    float x;
    int   hp;
    int   maxHp;
    float attackTimer;
    bool  isJumping;
    bool  isDucking;
    float actionTimer;
    float jumpHeight;
    bool  inMeleeCombat;
    bool  alive;
};

struct EnemyUnit {
    EnemyType type;
    int   lane;
    float x;
    int   hp;
    int   maxHp;
    float attackTimer;
    bool  inMeleeCombat;
    bool  alive;
};

struct Projectile {
    int   lane;
    float x;
    bool  fromPlayer;
    bool  isHigh;
    int   damage;
    float hitAnimTimer;
    bool  hitting;
    bool  alive;
};

struct Collectible {
    int   lane;
    float x;
    int   value;
    bool  alive;
};

enum GameState {
    STATE_MAIN_MENU = 0,
    STATE_CUTSCENE = 1,
    STATE_PLAYING = 2,
    STATE_PAUSED = 3,
    STATE_WIN = 4,
    STATE_LOSE = 5,
    STATE_SOUND_SETTINGS = 6
};

const int NUM_LANES = 4;
const float LANE_BOTTOM_Y[NUM_LANES] = { 0.20f, -0.15f, -0.50f, -0.85f };
const float LANE_TOP_Y[NUM_LANES] = { 0.55f,  0.20f, -0.15f, -0.50f };
const float LANE_CENTER_Y[NUM_LANES] = { 0.375f, 0.025f, -0.325f, -0.675f };
const float LANE_LEFT_X = -0.78f;
const float LANE_RIGHT_X = 0.95f;
const int PLAYER_COST[3] = { 10, 20, 30 };

extern GameState g_gameState;
extern bool g_laneSelected[NUM_LANES];
extern int  g_lastSelectedLane;
extern int  g_coins;
extern bool g_hasEarnedExtraCoins;
extern int  g_selectedCard;
extern float g_lanePlacementX[NUM_LANES];

extern std::vector<PlayerUnit>  g_players;
extern std::vector<EnemyUnit>   g_enemies;
extern std::vector<Projectile>  g_projectiles;
extern std::vector<Collectible> g_collectibles;

extern int   g_currentWave;
extern int   g_enemiesToSpawn;
extern float g_spawnTimer;

inline bool isCardUnlocked(int cardIdx, int coins, bool hasEarnedExtraCoins)
{
    int playerCost[3] = { 10, 20, 30 };
    if (cardIdx == 0) return coins >= playerCost[0];
    if (!hasEarnedExtraCoins) return false;
    return coins >= playerCost[cardIdx];
}

inline void spawnEnemyForCurrentWave(int currentWave, std::vector<EnemyUnit>& enemies, float rightX)
{
    EnemyUnit e;
    e.lane = std::rand() % 4;
    e.x = rightX;
    e.inMeleeCombat = false;
    e.alive = true;
    e.attackTimer = 0.8f;

    if (currentWave == 1) {
        e.type = ENEMY_EASY_UPPER;
    }
    else if (currentWave == 2) {
        e.type = (std::rand() % 2 == 0) ? ENEMY_EASY_UPPER : ENEMY_MEDIUM_LOWER;
    }
    else {
        int r = std::rand() % 3;
        e.type = (r == 0) ? ENEMY_EASY_UPPER : ((r == 1) ? ENEMY_MEDIUM_LOWER : ENEMY_HARD_MELEE);
    }

    e.maxHp = (e.type == ENEMY_HARD_MELEE) ? 7 : 5;
    e.hp = e.maxHp;
    enemies.push_back(e);
}

inline void startWave(int wave)
{
    g_currentWave = wave;
    if (wave == 1) {
        g_enemiesToSpawn = 3;
    }
    else if (wave == 2) {
        g_enemiesToSpawn = 5;
    }
    else {
        g_enemiesToSpawn = 10;
    }
    g_spawnTimer = 1.5f;
}

inline void resetGame()
{
    g_players.clear();
    g_enemies.clear();
    g_projectiles.clear();
    g_collectibles.clear();
    g_coins = 30;
    g_hasEarnedExtraCoins = false;
    g_selectedCard = 0;
    g_lastSelectedLane = -1;
    for (int i = 0; i < NUM_LANES; ++i) {
        g_laneSelected[i] = false;
    }
    startWave(1);
}

inline void clearAllLanes()
{
    for (int i = 0; i < NUM_LANES; ++i) {
        g_laneSelected[i] = false;
    }
    g_lastSelectedLane = -1;
}

inline std::vector<int>& laneSelectionOrder()
{
    static std::vector<int> order;
    return order;
}

inline void toggleLaneSelection(int lane)
{
    if (lane < 0 || lane >= NUM_LANES) return;
    std::vector<int>& order = laneSelectionOrder();
    // Remove lanes that were cleared or toggled off before recording this click.
    for (int i = (int)order.size() - 1; i >= 0; --i) {
        if (!g_laneSelected[order[i]] || order[i] == lane) {
            order.erase(order.begin() + i);
        }
    }
    g_laneSelected[lane] = !g_laneSelected[lane];
    if (g_laneSelected[lane]) {
        order.push_back(lane);
        g_lastSelectedLane = lane;
    }
    else if (g_lastSelectedLane == lane) {
        g_lastSelectedLane = order.empty() ? -1 : order.back();
    }
}

inline void spawnPlayerInLane(int lane, PlayerType type, float placementX = -0.35f)
{
    int cost = PLAYER_COST[(int)type];
    if (!isCardUnlocked((int)type, g_coins, g_hasEarnedExtraCoins) || g_coins < cost) return;

    g_coins -= cost;

    PlayerUnit p;
    p.type = type;
    p.lane = lane;
    p.x = LANE_LEFT_X + 0.04f;
    if (type == PLAYER_DEFENSE) {
        p.x = placementX;
        if (p.x < LANE_LEFT_X) p.x = LANE_LEFT_X;
        if (p.x > LANE_RIGHT_X) p.x = LANE_RIGHT_X;
    }
    p.maxHp = (type == PLAYER_DEFENSE) ? 7 : 5;
    p.hp = p.maxHp;
    p.attackTimer = 0.2f;
    p.isJumping = false;
    p.isDucking = false;
    p.actionTimer = 0.0f;
    p.jumpHeight = 0.0f;
    p.inMeleeCombat = false;
    p.alive = true;
    g_players.push_back(p);
}

inline void deploySelectedUnit()
{
    if (g_lastSelectedLane >= 0 && g_lastSelectedLane < NUM_LANES && g_laneSelected[g_lastSelectedLane]) {
        spawnPlayerInLane(g_lastSelectedLane, (PlayerType)g_selectedCard, g_lanePlacementX[g_lastSelectedLane]);
    }
    else {
        for (int i = 0; i < NUM_LANES; ++i) {
            if (g_laneSelected[i]) {
                spawnPlayerInLane(i, (PlayerType)g_selectedCard, g_lanePlacementX[i]);
                g_lastSelectedLane = i;
                break;
            }
        }
    }
}

inline void updateGameplay(float dt)
{
    if (g_enemiesToSpawn > 0) {
        g_spawnTimer -= dt;
        if (g_spawnTimer <= 0.0f) {
            spawnEnemyForCurrentWave(g_currentWave, g_enemies, LANE_RIGHT_X);
            g_enemiesToSpawn--;
            g_spawnTimer = 4.2f;
        }
    }
    else {
        bool anyEnemyAlive = false;
        for (int i = 0; i < (int)g_enemies.size(); ++i) {
            if (g_enemies[i].alive) {
                anyEnemyAlive = true;
                break;
            }
        }
        if (!anyEnemyAlive) {
            if (g_currentWave < 3) {
                startWave(g_currentWave + 1);
            }
            else {
                g_gameState = STATE_WIN;
                playAudioWin();
                return;
            }
        }
    }

    for (int i = 0; i < (int)g_players.size(); ++i) g_players[i].inMeleeCombat = false;
    for (int i = 0; i < (int)g_enemies.size(); ++i) g_enemies[i].inMeleeCombat = false;

    // Both Melee AND Ranged units collide with enemies upon reaching contact distance (0.085f)
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

        // Rise quickly, stay airborne, then land near the end of the jump.
        // Another jump press refreshes the timer without resetting the height.
        float targetHeight = (p.isJumping && p.actionTimer > 0.15f) ? 0.11f : 0.0f;
        if (p.jumpHeight < targetHeight) {
            p.jumpHeight += 0.8f * dt;
            if (p.jumpHeight > targetHeight) p.jumpHeight = targetHeight;
        }
        else if (p.jumpHeight > targetHeight) {
            p.jumpHeight -= 0.8f * dt;
            if (p.jumpHeight < targetHeight) p.jumpHeight = targetHeight;
        }

        // Check for alive enemies ahead in this specific lane
        bool hasEnemyInLane = false;
        for (int j = 0; j < (int)g_enemies.size(); ++j) {
            const EnemyUnit& e = g_enemies[j];
            if (e.alive && e.lane == p.lane && e.x >= p.x - 0.05f) {
                hasEnemyInLane = true;
                break;
            }
        }

        // Melee and ranged troops always advance unless an enemy is touching them.
        // Contact is checked again each frame, so they resume after the enemy dies.
        if (p.type != PLAYER_DEFENSE && !p.inMeleeCombat) {
            p.x += 0.14f * dt;
            if (p.x > LANE_RIGHT_X) p.x = LANE_RIGHT_X;
        }

        p.attackTimer -= dt;
        if (p.attackTimer <= 0.0f) {
            if (p.type == PLAYER_RANGED) {
                // Ranged archers shoot forward while advancing or in combat as long as an enemy is in their lane
                if (hasEnemyInLane) {
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
                    playAudioArrow();
                    p.attackTimer = 1.4f;
                }
                else {
                    p.attackTimer = 0.1f;
                }
            }
            else if (p.type == PLAYER_MELEE) {
                for (int j = 0; j < (int)g_enemies.size(); ++j) {
                    EnemyUnit& e = g_enemies[j];
                    if (e.alive && e.lane == p.lane && std::abs(p.x - e.x) <= 0.10f) {
                        e.hp -= 1;
                        playAudioHit();
                        if (e.hp <= 0) {
                            e.alive = false;
                            playAudioDeath();
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
                playAudioCoin();
            }
        }
    }

    for (int i = 0; i < (int)g_enemies.size(); ++i) {
        EnemyUnit& e = g_enemies[i];
        if (!e.alive) continue;

        float speed = (e.type == ENEMY_HARD_MELEE) ? 0.085f : 0.045f;
        if (!e.inMeleeCombat) {
            e.x -= speed * dt;
        }

        if (e.x <= LANE_LEFT_X) {
            g_gameState = STATE_LOSE;
            playAudioLose();
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
                e.attackTimer = 2.2f;
            }
            else if (e.type == ENEMY_HARD_MELEE) {
                for (int j = 0; j < (int)g_players.size(); ++j) {
                    PlayerUnit& p = g_players[j];
                    if (p.alive && p.lane == e.lane && std::abs(p.x - e.x) <= 0.10f) {
                        p.hp -= 2;
                        playAudioHit();
                        if (p.hp <= 0) {
                            p.alive = false;
                            playAudioDeath();
                        }
                        break;
                    }
                }
                e.attackTimer = 1.2f;
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

        proj.x += (proj.fromPlayer ? 0.55f : -0.40f) * dt;
        if (proj.x < LANE_LEFT_X || proj.x > LANE_RIGHT_X + 0.05f) {
            proj.alive = false;
            continue;
        }

        if (proj.fromPlayer) {
            for (int j = 0; j < (int)g_enemies.size(); ++j) {
                EnemyUnit& e = g_enemies[j];
                if (e.alive && e.lane == proj.lane && std::abs(proj.x - e.x) <= 0.045f) {
                    e.hp -= proj.damage;
                    playAudioHit();
                    proj.hitting = true;
                    proj.hitAnimTimer = 0.20f;
                    if (e.hp <= 0) {
                        e.alive = false;
                        playAudioDeath();
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
                    playAudioHit();
                    proj.alive = false;
                    if (p.hp <= 0) {
                        p.alive = false;
                        playAudioDeath();
                    }
                    break;
                }
            }
        }
    }
}
