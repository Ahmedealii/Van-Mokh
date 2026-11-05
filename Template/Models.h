#pragma once
#include "Mesh.h"

extern Mesh g_quadMesh;
extern Mesh g_triangleMesh;
extern Mesh g_circleMesh;
extern GLint locTransform;
extern GLint locHighlight;
extern GLint locRedTint;
extern GLint locGrayscale;
extern GLint locOverrideColor;
extern GLint locUseOverride;

enum PlayerType {
    PLAYER_MELEE = 0,
    PLAYER_RANGED = 1,
    PLAYER_DEFENSE = 2
};

enum EnemyType {
    ENEMY_EASY_UPPER = 0,
    ENEMY_MEDIUM_LOWER = 1,
    ENEMY_HARD_MELEE = 2
};

inline void drawPart(const Mesh& mesh, glm::mat4 parentTransform, glm::vec2 offset, glm::vec2 scale, float rotDeg, glm::vec3 color, float redTint = 0.0f, float highlight = 0.0f, int grayscale = 0)
{
    glm::mat4 model = glm::translate(parentTransform, glm::vec3(offset.x, offset.y, 0.0f));
    if (rotDeg != 0.0f) {
        model = glm::rotate(model, glm::radians(rotDeg), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

    glUniformMatrix4fv(locTransform, 1, GL_FALSE, glm::value_ptr(model));
    glUniform1f(locHighlight, highlight);
    glUniform1f(locRedTint, redTint);
    glUniform1i(locGrayscale, grayscale);
    glUniform1i(locUseOverride, 1);
    glUniform3fv(locOverrideColor, 1, glm::value_ptr(color));

    glBindVertexArray(mesh.vao);
    glDrawArrays(mesh.drawMode, 0, mesh.vertexCount);
}

inline void drawDigit(int digit, glm::vec2 pos, float scale, glm::vec3 col, int gray = 0)
{
    if (digit < 0 || digit > 9) return;
    glm::mat4 base = glm::mat4(1.0f);
    float w = 0.022f * scale;
    float t = 0.005f * scale;

    const int seg[10][7] = {
        { 1, 1, 1, 0, 1, 1, 1 }, // 0
        { 0, 0, 1, 0, 0, 1, 0 }, // 1
        { 1, 0, 1, 1, 1, 0, 1 }, // 2
        { 1, 0, 1, 1, 0, 1, 1 }, // 3
        { 0, 1, 1, 1, 0, 1, 0 }, // 4
        { 1, 1, 0, 1, 0, 1, 1 }, // 5
        { 1, 1, 0, 1, 1, 1, 1 }, // 6
        { 1, 0, 1, 0, 0, 1, 0 }, // 7
        { 1, 1, 1, 1, 1, 1, 1 }, // 8
        { 1, 1, 1, 1, 0, 1, 1 }  // 9
    };

    if (seg[digit][0]) drawPart(g_quadMesh, base, pos + glm::vec2(0.0f, w), glm::vec2(w, t), 0.0f, col, 0.0f, 0.0f, gray);
    if (seg[digit][1]) drawPart(g_quadMesh, base, pos + glm::vec2(-w * 0.5f, w * 0.5f), glm::vec2(t, w), 0.0f, col, 0.0f, 0.0f, gray);
    if (seg[digit][2]) drawPart(g_quadMesh, base, pos + glm::vec2(w * 0.5f, w * 0.5f), glm::vec2(t, w), 0.0f, col, 0.0f, 0.0f, gray);
    if (seg[digit][3]) drawPart(g_quadMesh, base, pos, glm::vec2(w, t), 0.0f, col, 0.0f, 0.0f, gray);
    if (seg[digit][4]) drawPart(g_quadMesh, base, pos + glm::vec2(-w * 0.5f, -w * 0.5f), glm::vec2(t, w), 0.0f, col, 0.0f, 0.0f, gray);
    if (seg[digit][5]) drawPart(g_quadMesh, base, pos + glm::vec2(w * 0.5f, -w * 0.5f), glm::vec2(t, w), 0.0f, col, 0.0f, 0.0f, gray);
    if (seg[digit][6]) drawPart(g_quadMesh, base, pos + glm::vec2(0.0f, -w), glm::vec2(w, t), 0.0f, col, 0.0f, 0.0f, gray);
}

inline void drawNumber(int number, glm::vec2 pos, float scale, glm::vec3 col, int gray = 0)
{
    if (number == 0) {
        drawDigit(0, pos, scale, col, gray);
        return;
    }
    std::vector<int> digits;
    int temp = std::abs(number);
    while (temp > 0) {
        digits.push_back(temp % 10);
        temp /= 10;
    }
    float step = 0.032f * scale;
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        int idx = (int)digits.size() - 1 - i;
        drawDigit(digits[i], pos + glm::vec2((float)idx * step, 0.0f), scale, col, gray);
    }
}

inline void drawHealthBar(glm::vec2 pos, int hp, int maxHp)
{
    glm::mat4 base(1.0f);
    float barW = 0.09f;
    float barH = 0.018f;
    drawPart(g_quadMesh, base, pos, glm::vec2(barW + 0.006f, barH + 0.006f), 0.0f, glm::vec3(0.1f, 0.1f, 0.1f));
    float segW = (barW / (float)maxHp) * 0.85f;
    float startX = pos.x - barW * 0.5f + (barW / (float)maxHp) * 0.5f;
    for (int i = 0; i < hp; ++i) {
        glm::vec2 segPos(startX + (float)i * (barW / (float)maxHp), pos.y);
        drawPart(g_quadMesh, base, segPos, glm::vec2(segW, barH * 0.8f), 0.0f, glm::vec3(0.2f, 0.9f, 0.25f));
    }
}

inline void drawHomeBase()
{
    glm::mat4 base(1.0f);
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, -0.15f), glm::vec2(0.18f, 1.45f), 0.0f, glm::vec3(0.45f, 0.47f, 0.52f));
    drawPart(g_quadMesh, base, glm::vec2(-0.88f, -0.15f), glm::vec2(0.12f, 1.38f), 0.0f, glm::vec3(0.55f, 0.57f, 0.62f));

    float cy_arr[4] = { 0.375f, 0.025f, -0.325f, -0.675f };
    for (int i = 0; i < 4; ++i) {
        float cy = cy_arr[i];
        drawPart(g_quadMesh, base, glm::vec2(-0.84f, cy - 0.02f), glm::vec2(0.06f, 0.16f), 0.0f, glm::vec3(0.42f, 0.25f, 0.12f));
        drawPart(g_circleMesh, base, glm::vec2(-0.84f, cy + 0.06f), glm::vec2(0.03f, 0.03f), 0.0f, glm::vec3(0.42f, 0.25f, 0.12f));
        drawPart(g_quadMesh, base, glm::vec2(-0.83f, cy), glm::vec2(0.01f, 0.16f), 0.0f, glm::vec3(0.2f, 0.2f, 0.22f));
    }
    for (int i = 0; i < 4; ++i) {
        drawPart(g_quadMesh, base, glm::vec2(-0.96f + (float)i * 0.045f, 0.61f), glm::vec2(0.03f, 0.06f), 0.0f, glm::vec3(0.45f, 0.47f, 0.52f));
    }
    drawPart(g_triangleMesh, base, glm::vec2(-0.90f, 0.72f), glm::vec2(0.20f, 0.18f), 0.0f, glm::vec3(0.15f, 0.35f, 0.80f));
}

inline void drawEnvironment(float time)
{
    glm::mat4 base(1.0f);
    drawPart(g_quadMesh, base, glm::vec2(0.1f, 0.62f), glm::vec2(1.8f, 0.14f), 0.0f, glm::vec3(0.32f, 0.33f, 0.37f));
    for (int i = 0; i < 12; ++i) {
        drawPart(g_quadMesh, base, glm::vec2(-0.70f + (float)i * 0.14f, 0.71f), glm::vec2(0.07f, 0.05f), 0.0f, glm::vec3(0.38f, 0.39f, 0.43f));
    }

    float towerX[3] = { -0.35f, 0.35f, 0.85f };
    for (int idx = 0; idx < 3; ++idx) {
        float tx = towerX[idx];
        drawPart(g_quadMesh, base, glm::vec2(tx, 0.72f), glm::vec2(0.14f, 0.28f), 0.0f, glm::vec3(0.42f, 0.44f, 0.48f));
        drawPart(g_triangleMesh, base, glm::vec2(tx, 0.91f), glm::vec2(0.18f, 0.12f), 0.0f, glm::vec3(0.60f, 0.20f, 0.18f));
        drawPart(g_quadMesh, base, glm::vec2(tx, 0.75f), glm::vec2(0.025f, 0.06f), 0.0f, glm::vec3(0.12f, 0.12f, 0.15f));
    }

    float bannerX[3] = { -0.55f, 0.0f, 0.60f };
    for (int idx = 0; idx < 3; ++idx) {
        float bx = bannerX[idx];
        drawPart(g_quadMesh, base, glm::vec2(bx, 0.63f), glm::vec2(0.05f, 0.09f), 0.0f, glm::vec3(0.15f, 0.35f, 0.85f));
        drawPart(g_triangleMesh, base, glm::vec2(bx, 0.56f), glm::vec2(0.05f, 0.05f), 180.0f, glm::vec3(0.15f, 0.35f, 0.85f));
        drawPart(g_quadMesh, base, glm::vec2(bx, 0.63f), glm::vec2(0.015f, 0.07f), 0.0f, glm::vec3(0.95f, 0.80f, 0.15f));
    }

    float flicker = std::sin(time * 10.0f) * 8.0f;
    float torchX[3] = { -0.18f, 0.18f, 0.72f };
    for (int idx = 0; idx < 3; ++idx) {
        float fx = torchX[idx];
        drawPart(g_quadMesh, base, glm::vec2(fx, 0.59f), glm::vec2(0.014f, 0.05f), 0.0f, glm::vec3(0.45f, 0.28f, 0.12f));
        drawPart(g_triangleMesh, base, glm::vec2(fx, 0.63f), glm::vec2(0.032f, 0.045f), flicker, glm::vec3(1.0f, 0.45f, 0.05f));
        drawPart(g_triangleMesh, base, glm::vec2(fx, 0.625f), glm::vec2(0.018f, 0.028f), -flicker, glm::vec3(1.0f, 0.90f, 0.20f));
    }

    // Clean bottom scenery: simple stones & pine trees in front of the lanes (no random sticks)
    for (int i = 0; i < 5; ++i) {
        float rx = -0.60f + (float)i * 0.35f;
        drawPart(g_circleMesh, base, glm::vec2(rx, -0.92f), glm::vec2(0.045f, 0.030f), 0.0f, glm::vec3(0.40f, 0.42f, 0.45f));
        drawPart(g_circleMesh, base, glm::vec2(rx + 0.03f, -0.93f), glm::vec2(0.030f, 0.022f), 0.0f, glm::vec3(0.50f, 0.52f, 0.55f));
        drawPart(g_quadMesh, base, glm::vec2(rx + 0.14f, -0.93f), glm::vec2(0.016f, 0.04f), 0.0f, glm::vec3(0.42f, 0.25f, 0.12f));
        drawPart(g_triangleMesh, base, glm::vec2(rx + 0.14f, -0.89f), glm::vec2(0.07f, 0.08f), 0.0f, glm::vec3(0.15f, 0.48f, 0.20f));
        drawPart(g_triangleMesh, base, glm::vec2(rx + 0.14f, -0.85f), glm::vec2(0.055f, 0.07f), 0.0f, glm::vec3(0.20f, 0.58f, 0.25f));
    }
}

inline void drawPlayerModel(PlayerType type, glm::vec2 pos, float time, float redTint, bool jumping, bool ducking, int gray = 0)
{
    float yOffset = jumping ? 0.075f : (ducking ? -0.035f : 0.0f);
    float yScale = ducking ? 0.62f : 1.0f;

    glm::mat4 base = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y + yOffset, 0.0f));
    base = glm::scale(base, glm::vec3(1.0f, yScale, 1.0f));

    float walkBob = (type == PLAYER_DEFENSE) ? 0.0f : std::sin(time * 8.0f) * 12.0f;

    if (type == PLAYER_MELEE) {
        drawPart(g_quadMesh, base, glm::vec2(-0.015f, -0.07f), glm::vec2(0.015f, 0.05f), walkBob, glm::vec3(0.3f, 0.3f, 0.35f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.015f, -0.07f), glm::vec2(0.015f, 0.05f), -walkBob, glm::vec3(0.3f, 0.3f, 0.35f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.01f), glm::vec2(0.055f, 0.08f), 0.0f, glm::vec3(0.18f, 0.45f, 0.88f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.01f), glm::vec2(0.02f, 0.07f), 0.0f, glm::vec3(0.85f, 0.85f, 0.90f), redTint, 0.0f, gray);
        drawPart(g_circleMesh, base, glm::vec2(0.0f, 0.055f), glm::vec2(0.032f, 0.032f), 0.0f, glm::vec3(0.75f, 0.78f, 0.82f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.012f, 0.055f), glm::vec2(0.02f, 0.008f), 0.0f, glm::vec3(0.15f, 0.15f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_triangleMesh, base, glm::vec2(-0.01f, 0.095f), glm::vec2(0.03f, 0.035f), 15.0f, glm::vec3(0.95f, 0.80f, 0.15f), redTint, 0.0f, gray);

        float swingAngle = -35.0f + std::sin(time * 9.0f) * 40.0f;
        glm::mat4 handMat = glm::translate(base, glm::vec3(0.025f, 0.0f, 0.0f));
        handMat = glm::rotate(handMat, glm::radians(swingAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        drawPart(g_quadMesh, handMat, glm::vec2(0.035f, 0.0f), glm::vec2(0.07f, 0.014f), 0.0f, glm::vec3(0.88f, 0.90f, 0.95f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, handMat, glm::vec2(0.005f, 0.0f), glm::vec2(0.012f, 0.038f), 0.0f, glm::vec3(0.90f, 0.75f, 0.15f), redTint, 0.0f, gray);
    }
    else if (type == PLAYER_RANGED) {
        drawPart(g_quadMesh, base, glm::vec2(-0.015f, -0.07f), glm::vec2(0.014f, 0.05f), walkBob, glm::vec3(0.40f, 0.25f, 0.12f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.015f, -0.07f), glm::vec2(0.014f, 0.05f), -walkBob, glm::vec3(0.40f, 0.25f, 0.12f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(-0.03f, 0.01f), glm::vec2(0.02f, 0.06f), 20.0f, glm::vec3(0.55f, 0.35f, 0.15f), redTint, 0.0f, gray);
        drawPart(g_triangleMesh, base, glm::vec2(0.0f, -0.005f), glm::vec2(0.065f, 0.09f), 0.0f, glm::vec3(0.15f, 0.72f, 0.55f), redTint, 0.0f, gray);
        drawPart(g_circleMesh, base, glm::vec2(0.0f, 0.055f), glm::vec2(0.028f, 0.028f), 0.0f, glm::vec3(0.95f, 0.78f, 0.62f), redTint, 0.0f, gray);
        drawPart(g_triangleMesh, base, glm::vec2(-0.005f, 0.075f), glm::vec2(0.065f, 0.045f), 10.0f, glm::vec3(0.10f, 0.55f, 0.42f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, 0.025f), glm::vec2(0.01f, 0.055f), 25.0f, glm::vec3(0.65f, 0.40f, 0.15f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, -0.025f), glm::vec2(0.01f, 0.055f), -25.0f, glm::vec3(0.65f, 0.40f, 0.15f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.028f, 0.0f), glm::vec2(0.004f, 0.095f), 0.0f, glm::vec3(0.90f, 0.90f, 0.90f), redTint, 0.0f, gray);
    }
    else if (type == PLAYER_DEFENSE) {
        drawPart(g_quadMesh, base, glm::vec2(-0.02f, -0.07f), glm::vec2(0.022f, 0.05f), 0.0f, glm::vec3(0.28f, 0.30f, 0.35f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.01f, -0.07f), glm::vec2(0.022f, 0.05f), 0.0f, glm::vec3(0.28f, 0.30f, 0.35f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(-0.005f, -0.005f), glm::vec2(0.07f, 0.09f), 0.0f, glm::vec3(0.50f, 0.54f, 0.62f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(-0.005f, 0.06f), glm::vec2(0.05f, 0.05f), 0.0f, glm::vec3(0.65f, 0.68f, 0.75f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.005f, 0.06f), glm::vec2(0.03f, 0.01f), 0.0f, glm::vec3(0.1f, 0.1f, 0.1f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.042f, -0.005f), glm::vec2(0.038f, 0.145f), 0.0f, glm::vec3(0.88f, 0.72f, 0.15f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.042f, -0.005f), glm::vec2(0.024f, 0.125f), 0.0f, glm::vec3(0.15f, 0.38f, 0.82f), redTint, 0.0f, gray);
        drawPart(g_circleMesh, base, glm::vec2(0.042f, -0.005f), glm::vec2(0.015f, 0.015f), 0.0f, glm::vec3(0.95f, 0.85f, 0.25f), redTint, 0.0f, gray);
    }
}

inline void drawEnemyModel(EnemyType type, glm::vec2 pos, float time, float redTint)
{
    glm::mat4 base = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
    float walkBob = std::sin(time * 9.0f) * 14.0f;

    if (type == ENEMY_EASY_UPPER) {
        float hoverY = 0.035f + std::sin(time * 5.0f) * 0.015f;
        glm::mat4 flyMat = glm::translate(base, glm::vec3(0.0f, hoverY, 0.0f));
        float wingFlap = std::sin(time * 12.0f) * 22.0f;
        drawPart(g_triangleMesh, flyMat, glm::vec2(0.02f, 0.03f), glm::vec2(0.07f, 0.06f), -30.0f + wingFlap, glm::vec3(0.45f, 0.12f, 0.55f), redTint);
        drawPart(g_circleMesh, flyMat, glm::vec2(0.0f, 0.0f), glm::vec2(0.036f, 0.042f), 0.0f, glm::vec3(0.68f, 0.20f, 0.72f), redTint);
        drawPart(g_circleMesh, flyMat, glm::vec2(-0.025f, 0.035f), glm::vec2(0.026f, 0.026f), 0.0f, glm::vec3(0.75f, 0.25f, 0.78f), redTint);
        drawPart(g_triangleMesh, flyMat, glm::vec2(-0.02f, 0.068f), glm::vec2(0.018f, 0.03f), 15.0f, glm::vec3(0.95f, 0.85f, 0.20f), redTint);
        drawPart(g_circleMesh, flyMat, glm::vec2(-0.035f, 0.038f), glm::vec2(0.008f, 0.008f), 0.0f, glm::vec3(1.0f, 0.9f, 0.1f), redTint);
    }
    else if (type == ENEMY_MEDIUM_LOWER) {
        drawPart(g_quadMesh, base, glm::vec2(-0.015f, -0.075f), glm::vec2(0.016f, 0.04f), walkBob, glm::vec3(0.35f, 0.45f, 0.15f), redTint);
        drawPart(g_quadMesh, base, glm::vec2(0.015f, -0.075f), glm::vec2(0.016f, 0.04f), -walkBob, glm::vec3(0.35f, 0.45f, 0.15f), redTint);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.03f), glm::vec2(0.06f, 0.065f), 0.0f, glm::vec3(0.45f, 0.28f, 0.14f), redTint);
        drawPart(g_circleMesh, base, glm::vec2(-0.01f, 0.02f), glm::vec2(0.028f, 0.026f), 0.0f, glm::vec3(0.42f, 0.68f, 0.22f), redTint);
        drawPart(g_triangleMesh, base, glm::vec2(0.015f, 0.035f), glm::vec2(0.035f, 0.02f), -40.0f, glm::vec3(0.42f, 0.68f, 0.22f), redTint);
        drawPart(g_quadMesh, base, glm::vec2(-0.045f, -0.045f), glm::vec2(0.065f, 0.016f), 0.0f, glm::vec3(0.30f, 0.20f, 0.10f), redTint);
        drawPart(g_quadMesh, base, glm::vec2(-0.065f, -0.045f), glm::vec2(0.01f, 0.045f), 0.0f, glm::vec3(0.70f, 0.70f, 0.75f), redTint);
    }
    else if (type == ENEMY_HARD_MELEE) {
        drawPart(g_quadMesh, base, glm::vec2(-0.02f, -0.07f), glm::vec2(0.022f, 0.055f), walkBob * 1.4f, glm::vec3(0.25f, 0.12f, 0.12f), redTint);
        drawPart(g_quadMesh, base, glm::vec2(0.02f, -0.07f), glm::vec2(0.022f, 0.055f), -walkBob * 1.4f, glm::vec3(0.25f, 0.12f, 0.12f), redTint);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.0f), glm::vec2(0.08f, 0.10f), 0.0f, glm::vec3(0.65f, 0.18f, 0.15f), redTint);
        drawPart(g_triangleMesh, base, glm::vec2(0.03f, 0.055f), glm::vec2(0.045f, 0.04f), -20.0f, glm::vec3(0.25f, 0.25f, 0.28f), redTint);
        drawPart(g_circleMesh, base, glm::vec2(-0.005f, 0.07f), glm::vec2(0.034f, 0.034f), 0.0f, glm::vec3(0.55f, 0.15f, 0.12f), redTint);
        drawPart(g_triangleMesh, base, glm::vec2(-0.02f, 0.105f), glm::vec2(0.022f, 0.035f), 25.0f, glm::vec3(0.9f, 0.88f, 0.8f), redTint);
        drawPart(g_triangleMesh, base, glm::vec2(0.01f, 0.105f), glm::vec2(0.022f, 0.035f), -25.0f, glm::vec3(0.9f, 0.88f, 0.8f), redTint);

        float axeSwing = 30.0f + std::sin(time * 11.0f) * 45.0f;
        glm::mat4 axeMat = glm::translate(base, glm::vec3(-0.035f, 0.01f, 0.0f));
        axeMat = glm::rotate(axeMat, glm::radians(axeSwing), glm::vec3(0.0f, 0.0f, 1.0f));
        drawPart(g_quadMesh, axeMat, glm::vec2(-0.03f, 0.02f), glm::vec2(0.012f, 0.09f), 30.0f, glm::vec3(0.45f, 0.28f, 0.12f), redTint);
        drawPart(g_triangleMesh, axeMat, glm::vec2(-0.055f, 0.045f), glm::vec2(0.05f, 0.045f), 120.0f, glm::vec3(0.80f, 0.82f, 0.85f), redTint);
    }
}