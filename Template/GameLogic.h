#pragma once
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
    STATE_LOSE = 5
};

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
    e.attackTimer = 0.4f;

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