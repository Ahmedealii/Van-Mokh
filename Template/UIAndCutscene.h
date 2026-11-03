#pragma once
#include "Models.h"
#include "GameLogic.h"

inline void drawCollectibleCoin(glm::vec2 pos, float time)
{
    glm::mat4 base(1.0f);
    float pulse = 1.0f + 0.12f * std::sin(time * 6.0f);
    drawPart(g_circleMesh, base, pos, glm::vec2(0.028f * pulse, 0.028f * pulse), 0.0f, glm::vec3(0.98f, 0.82f, 0.12f));
    drawPart(g_circleMesh, base, pos, glm::vec2(0.020f * pulse, 0.020f * pulse), 0.0f, glm::vec3(1.00f, 0.92f, 0.40f));
    drawPart(g_quadMesh, base, pos, glm::vec2(0.008f * pulse, 0.022f * pulse), 0.0f, glm::vec3(0.85f, 0.65f, 0.05f));
}

inline void drawProjectile(const Projectile& p, float time)
{
    glm::mat4 base(1.0f);
    float lane_center_y[4] = { 0.375f, 0.025f, -0.325f, -0.675f };
    float y = lane_center_y[p.lane];
    if (!p.fromPlayer) {
        y += p.isHigh ? 0.055f : -0.055f;
    }

    if (p.hitting) {
        float s = 1.0f + (0.25f - p.hitAnimTimer) * 6.0f;
        drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.025f * s, 0.025f * s), 0.0f, glm::vec3(1.0f, 0.85f, 0.2f));
        drawPart(g_triangleMesh, base, glm::vec2(p.x, y), glm::vec2(0.04f * s, 0.04f * s), time * 360.0f, glm::vec3(1.0f, 0.4f, 0.1f));
        return;
    }

    if (p.fromPlayer) {
        drawPart(g_quadMesh, base, glm::vec2(p.x, y), glm::vec2(0.045f, 0.007f), 0.0f, glm::vec3(0.65f, 0.42f, 0.18f));
        drawPart(g_triangleMesh, base, glm::vec2(p.x + 0.025f, y), glm::vec2(0.02f, 0.022f), -90.0f, glm::vec3(0.85f, 0.88f, 0.92f));
        drawPart(g_quadMesh, base, glm::vec2(p.x - 0.02f, y), glm::vec2(0.012f, 0.016f), 0.0f, glm::vec3(0.95f, 0.95f, 0.95f));
    }
    else {
        glm::vec3 col = p.isHigh ? glm::vec3(0.95f, 0.25f, 0.85f) : glm::vec3(0.95f, 0.45f, 0.10f);
        drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.018f, 0.018f), 0.0f, col);
        drawPart(g_triangleMesh, base, glm::vec2(p.x + 0.02f, y), glm::vec2(0.025f, 0.022f), -90.0f, glm::vec3(1.0f, 0.9f, 0.2f));
    }
}

inline bool isPointInBox(glm::vec2 pt, glm::vec2 center, glm::vec2 size)
{
    return (pt.x >= center.x - size.x * 0.5f && pt.x <= center.x + size.x * 0.5f &&
        pt.y >= center.y - size.y * 0.5f && pt.y <= center.y + size.y * 0.5f);
}

inline void drawButton(glm::vec2 center, glm::vec2 size, glm::vec3 color, int labelCode, glm::vec2 cursorNDC)
{
    glm::mat4 base(1.0f);
    bool hovered = isPointInBox(cursorNDC, center, size);
    drawPart(g_quadMesh, base, center, size + glm::vec2(0.015f), 0.0f, glm::vec3(0.9f, 0.8f, 0.3f), 0.0f, hovered ? 0.5f : 0.0f);
    drawPart(g_quadMesh, base, center, size, 0.0f, color, 0.0f, hovered ? 0.35f : 0.0f);

    if (labelCode == 1) {
        drawPart(g_triangleMesh, base, center, glm::vec2(0.06f, 0.06f), -90.0f, glm::vec3(0.95f, 0.95f, 0.95f));
    }
    else if (labelCode == 2) {
        drawPart(g_circleMesh, base, center, glm::vec2(0.035f, 0.035f), 0.0f, glm::vec3(0.95f, 0.95f, 0.95f));
        drawPart(g_circleMesh, base, center, glm::vec2(0.020f, 0.020f), 0.0f, color);
    }
    else if (labelCode == 3) {
        drawPart(g_quadMesh, base, center, glm::vec2(0.06f, 0.014f), 45.0f, glm::vec3(0.95f, 0.95f, 0.95f));
        drawPart(g_quadMesh, base, center, glm::vec2(0.06f, 0.014f), -45.0f, glm::vec3(0.95f, 0.95f, 0.95f));
    }
}

inline void renderTopBarUI(float time, int coins, bool hasEarnedExtraCoins, int selectedCard, int hoveredCard, int currentWave)
{
    int playerCost[3] = { 10, 20, 30 };
    glm::mat4 base(1.0f);
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.87f), glm::vec2(2.0f, 0.26f), 0.0f, glm::vec3(0.16f, 0.18f, 0.22f));

    for (int i = 0; i < 3; ++i) {
        glm::vec2 cardCenter(-0.68f + i * 0.24f, 0.87f);
        glm::vec2 cardSize(0.20f, 0.21f);
        bool unlocked = isCardUnlocked(i, coins, hasEarnedExtraCoins);
        int gray = unlocked ? 0 : 1;
        float highlight = (unlocked && (selectedCard == i || hoveredCard == i)) ? 0.35f : 0.0f;

        drawPart(g_quadMesh, base, cardCenter, cardSize, 0.0f, glm::vec3(0.45f, 0.38f, 0.22f), 0.0f, highlight, gray);
        drawPart(g_quadMesh, base, cardCenter, cardSize - glm::vec2(0.018f), 0.0f, glm::vec3(0.28f, 0.30f, 0.35f), 0.0f, highlight, gray);

        drawPlayerModel((PlayerType)i, cardCenter + glm::vec2(0.0f, 0.02f), unlocked ? time : 0.0f, 0.0f, false, false, gray);

        drawPart(g_circleMesh, base, cardCenter + glm::vec2(-0.055f, -0.068f), glm::vec2(0.016f, 0.016f), 0.0f, glm::vec3(0.98f, 0.80f, 0.15f), 0.0f, 0.0f, gray);
        drawNumber(playerCost[i], cardCenter + glm::vec2(-0.015f, -0.068f), 0.85f, glm::vec3(1.0f, 0.95f, 0.8f), gray);
    }

    drawPart(g_quadMesh, base, glm::vec2(0.20f, 0.87f), glm::vec2(0.18f, 0.12f), 0.0f, glm::vec3(0.25f, 0.28f, 0.35f));
    drawNumber(currentWave, glm::vec2(0.20f, 0.87f), 1.4f, glm::vec3(0.4f, 0.85f, 1.0f));

    drawCollectibleCoin(glm::vec2(0.58f, 0.87f), time);
    drawNumber(coins, glm::vec2(0.66f, 0.87f), 1.8f, glm::vec3(1.0f, 0.85f, 0.15f));
}

inline void renderCutscene(float time, float cutsceneTimer)
{
    drawEnvironment(time);
    drawHomeBase();

    glm::mat4 base(1.0f);
    drawPart(g_quadMesh, base, glm::vec2(0.08f, -0.15f), glm::vec2(1.85f, 1.40f), 0.0f, glm::vec3(0.28f, 0.58f, 0.22f));

    float crystalBob = std::sin(time * 4.0f) * 0.02f;
    drawPart(g_triangleMesh, base, glm::vec2(-0.72f, 0.05f + crystalBob), glm::vec2(0.08f, 0.12f), 0.0f, glm::vec3(0.2f, 0.85f, 1.0f));
    drawPart(g_triangleMesh, base, glm::vec2(-0.72f, -0.07f + crystalBob), glm::vec2(0.08f, 0.12f), 180.0f, glm::vec3(0.15f, 0.65f, 0.95f));

    float knightX = -0.50f + (cutsceneTimer * 0.04f < 0.25f ? cutsceneTimer * 0.04f : 0.25f);
    float enemyX = 0.75f - (cutsceneTimer * 0.06f < 0.45f ? cutsceneTimer * 0.06f : 0.45f);

    drawPlayerModel(PLAYER_MELEE, glm::vec2(knightX, 0.0f), time * 1.5f, 0.0f, (cutsceneTimer > 6.0f && cutsceneTimer < 7.0f), false);
    drawPlayerModel(PLAYER_DEFENSE, glm::vec2(-0.60f, -0.25f), time, 0.0f, false, false);
    drawEnemyModel(ENEMY_HARD_MELEE, glm::vec2(enemyX, 0.0f), time * 1.5f, 0.0f);
    drawEnemyModel(ENEMY_EASY_UPPER, glm::vec2(enemyX + 0.15f, 0.20f), time * 1.5f, 0.0f);

    if (cutsceneTimer > 5.0f && cutsceneTimer < 9.0f) {
        Projectile warningShot;
        warningShot.lane = 1;
        warningShot.x = enemyX - (cutsceneTimer - 5.0f) * 0.25f;
        warningShot.fromPlayer = false;
        warningShot.isHigh = false;
        warningShot.damage = 1;
        warningShot.hitAnimTimer = 0.0f;
        warningShot.hitting = false;
        warningShot.alive = true;
        drawProjectile(warningShot, time);
    }

    drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.78f), glm::vec2(1.65f, 0.24f), 0.0f, glm::vec3(0.10f, 0.12f, 0.16f));
    drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.78f), glm::vec2(1.61f, 0.20f), 0.0f, glm::vec3(0.18f, 0.20f, 0.26f));

    if (cutsceneTimer < 4.5f) {
        drawEnemyModel(ENEMY_HARD_MELEE, glm::vec2(-0.68f, -0.78f), time, 0.0f);
        for (int i = 0; i < 8; ++i)
            drawPart(g_quadMesh, base, glm::vec2(-0.45f + i * 0.12f, -0.76f), glm::vec2(0.09f, 0.025f), 0.0f, glm::vec3(0.95f, 0.35f, 0.35f));
    }
    else if (cutsceneTimer < 9.5f) {
        drawPlayerModel(PLAYER_MELEE, glm::vec2(-0.68f, -0.78f), time, 0.0f, false, false);
        for (int i = 0; i < 8; ++i)
            drawPart(g_quadMesh, base, glm::vec2(-0.45f + i * 0.12f, -0.76f), glm::vec2(0.09f, 0.025f), 0.0f, glm::vec3(0.35f, 0.75f, 1.0f));
    }
    else {
        drawPlayerModel(PLAYER_RANGED, glm::vec2(-0.68f, -0.78f), time, 0.0f, false, false);
        for (int i = 0; i < 6; ++i)
            drawPart(g_quadMesh, base, glm::vec2(-0.45f + i * 0.15f, -0.76f), glm::vec2(0.11f, 0.03f), 0.0f, glm::vec3(1.0f, 0.85f, 0.2f));
    }
}