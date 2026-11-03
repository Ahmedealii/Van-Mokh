#pragma once
#include "Mesh.h"
#include <vector>
#include <cmath>
#include <algorithm>

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

    drawPart(g_quadMesh, base, pos + glm::vec2(0.0f, w), glm::vec2(w, t), 0.0f, col, 0.0f, 0.0f, gray);
    drawPart(g_quadMesh, base, pos + glm::vec2(-w * 0.5f, w * 0.5f), glm::vec2(t, w * 0.5f), 0.0f, col, 0.0f, 0.0f, gray);
    drawPart(g_quadMesh, base, pos + glm::vec2(w * 0.5f, w * 0.5f), glm::vec2(t, w * 0.5f), 0.0f, col, 0.0f, 0.0f, gray);
    drawPart(g_quadMesh, base, pos, glm::vec2(w, t), 0.0f, col, 0.0f, 0.0f, gray);
    drawPart(g_quadMesh, base, pos + glm::vec2(-w * 0.5f, -w * 0.5f), glm::vec2(t, w * 0.5f), 0.0f, col, 0.0f, 0.0f, gray);
    drawPart(g_quadMesh, base, pos + glm::vec2(w * 0.5f, -w * 0.5f), glm::vec2(t, w * 0.5f), 0.0f, col, 0.0f, 0.0f, gray);
    drawPart(g_quadMesh, base, pos + glm::vec2(0.0f, -w), glm::vec2(w, t), 0.0f, col, 0.0f, 0.0f, gray);
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
        drawDigit(digits[i], pos + glm::vec2(idx * step, 0.0f), scale, col, gray);
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
        glm::vec2 segPos(startX + i * (barW / (float)maxHp), pos.y);
        drawPart(g_quadMesh, base, segPos, glm::vec2(segW, barH * 0.8f), 0.0f, glm::vec3(0.2f, 0.9f, 0.25f));
    }
}

inline void drawHomeBase()
{
    glm::mat4 base(1.0f);

    // tower body
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, -0.15f), glm::vec2(0.18f, 1.45f), 0.0f, glm::vec3(0.15f, 0.17f, 0.20f));
    drawPart(g_quadMesh, base, glm::vec2(-0.88f, -0.15f), glm::vec2(0.12f, 1.38f), 0.0f, glm::vec3(0.30f, 0.32f, 0.38f));

    // windows
    float slotY[4] = { 0.375f, 0.025f, -0.325f, -0.675f };
    for (int i = 0; i < 4; ++i) {
        float cy = slotY[i];
        drawPart(g_quadMesh, base, glm::vec2(-0.84f, cy), glm::vec2(0.06f, 0.16f), 0.0f, glm::vec3(0.02f, 0.02f, 0.04f));
        drawPart(g_quadMesh, base, glm::vec2(-0.84f, cy), glm::vec2(0.04f, 0.14f), 0.0f, glm::vec3(0.3f, 0.6f, 1.0f));
        drawPart(g_quadMesh, base, glm::vec2(-0.84f, cy + 0.08f), glm::vec2(0.07f, 0.01f), 0.0f, glm::vec3(0.4f, 0.4f, 0.4f));
        drawPart(g_quadMesh, base, glm::vec2(-0.84f, cy - 0.08f), glm::vec2(0.07f, 0.01f), 0.0f, glm::vec3(0.4f, 0.4f, 0.4f));
    }

    // bricks
    for (int i = 0; i < 12; ++i) {
        float ry = 0.6f - (i * 0.12f);
        drawPart(g_quadMesh, base, glm::vec2(-0.90f, ry), glm::vec2(0.18f, 0.005f), 0.0f, glm::vec3(0.1f, 0.1f, 0.12f));
    }

    // top
    for (int i = 0; i < 3; ++i) {
        drawPart(g_quadMesh, base, glm::vec2(-0.96f + i * 0.07f, 0.78f), glm::vec2(0.05f, 0.08f), 0.0f, glm::vec3(0.25f, 0.25f, 0.30f));
    }

    // roof
    drawPart(g_triangleMesh, base, glm::vec2(-0.90f, 0.72f), glm::vec2(0.20f, 0.18f), 0.0f, glm::vec3(0.05f, 0.10f, 0.40f));
}

// Grass detail for ONE lane: checker tiles, grass tufts and flowers.
// Call it right after the plain lane rectangle of that lane is drawn.
// Pass the same highlight value as the lane so hovering/selecting still tints the tiles.
inline void drawLaneGrass(int laneIndex, float cy, float h, float xLeft, float xRight, glm::vec3 laneCol, float highlight)
{
    glm::mat4 base(1.0f);
    float laneW = xRight - xLeft;
    float xMid = (xLeft + xRight) * 0.5f;

    // lighter tiles on every other cell, so the lane reads as a row of grid cells
    int cells = 9;
    float cellW = laneW / (float)cells;
    for (int c = 0; c < cells; ++c) {
        if ((c + laneIndex) % 2 == 0) continue;
        float cx = xLeft + (c + 0.5f) * cellW;
        drawPart(g_quadMesh, base, glm::vec2(cx, cy), glm::vec2(cellW, h - 0.01f), 0.0f, laneCol * 1.10f, 0.0f, highlight);
    }

    // thin darker strip along the top edge of the lane
    drawPart(g_quadMesh, base, glm::vec2(xMid, cy + (h - 0.01f) * 0.5f - 0.004f), glm::vec2(laneW, 0.008f), 0.0f, laneCol * 0.75f, 0.0f, highlight);

    // grass tufts (3 blades each), fixed positions so nothing flickers
    float yRange = h - 0.08f;
    for (int j = 0; j < 36; ++j) {
        float r1 = std::fabs(std::sin(j * 12.9898f + laneIndex * 78.233f));
        float r2 = std::fabs(std::sin(j * 39.346f + laneIndex * 11.135f));
        float tx = xLeft + 0.03f + r1 * (laneW - 0.06f);
        float ty = cy + (r2 - 0.5f) * yRange;
        glm::vec3 blade = (j % 3 == 0) ? laneCol * 1.35f : laneCol * 0.70f;
        drawPart(g_triangleMesh, base, glm::vec2(tx - 0.007f, ty), glm::vec2(0.008f, 0.026f), -18.0f, blade, 0.0f, highlight);
        drawPart(g_triangleMesh, base, glm::vec2(tx, ty + 0.003f), glm::vec2(0.008f, 0.032f), 0.0f, blade, 0.0f, highlight);
        drawPart(g_triangleMesh, base, glm::vec2(tx + 0.007f, ty), glm::vec2(0.008f, 0.026f), 18.0f, blade, 0.0f, highlight);
    }

    // a few flowers
    for (int f = 0; f < 4; ++f) {
        float r1 = std::fabs(std::sin(f * 5.731f + laneIndex * 3.17f + 1.0f));
        float r2 = std::fabs(std::sin(f * 9.173f + laneIndex * 7.77f + 2.0f));
        float fx = xLeft + 0.08f + r1 * (laneW - 0.16f);
        float fy = cy + (r2 - 0.5f) * (yRange - 0.04f);
        glm::vec3 petal = (f % 2 == 0) ? glm::vec3(0.95f, 0.95f, 0.90f) : glm::vec3(0.98f, 0.85f, 0.25f);
        drawPart(g_circleMesh, base, glm::vec2(fx, fy), glm::vec2(0.014f, 0.014f), 0.0f, petal, 0.0f, highlight);
        drawPart(g_circleMesh, base, glm::vec2(fx, fy), glm::vec2(0.006f, 0.006f), 0.0f, glm::vec3(0.90f, 0.50f, 0.10f), 0.0f, highlight);
    }
}

inline void drawEnvironment(float time)
{
    (void)time;   // nothing in the environment is animated any more
    glm::mat4 base(1.0f);

    // plain grass lanes (used by the menu background; gameplay lanes are drawn in main.cpp)
    float laneY[4] = { 0.375f, 0.025f, -0.325f, -0.675f };
    glm::vec3 colors[4] = {
        glm::vec3(0.30f, 0.50f, 0.15f),
        glm::vec3(0.40f, 0.60f, 0.25f),
        glm::vec3(0.35f, 0.55f, 0.20f),
        glm::vec3(0.25f, 0.45f, 0.10f)
    };

    for (int i = 0; i < 4; ++i) {
        drawPart(g_quadMesh, base, glm::vec2(0.0f, laneY[i]), glm::vec2(2.0f, 0.3f), 0.0f, colors[i]);
    }

    // sky/background (the blue moat behind the wall)
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.60f), glm::vec2(2.0f, 0.4f), 0.0f, glm::vec3(0.25f, 0.35f, 0.55f));

    // wall
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.80f), glm::vec2(2.0f, 0.3f), 0.0f, glm::vec3(0.3f, 0.3f, 0.35f));

    // ===== TOP SECTION (simple and mirrored around x = 0) =====

    // dark strip along the top edge of the wall
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.95f), glm::vec2(2.0f, 0.02f), 0.0f, glm::vec3(0.1f, 0.1f, 0.15f));

    // merlons along the wall top: same spacing on both sides of the centre
    for (int i = 0; i < 7; ++i) {
        float mx = 0.07f + i * 0.14f;
        drawPart(g_quadMesh, base, glm::vec2(mx, 0.98f), glm::vec2(0.07f, 0.06f), 0.0f, glm::vec3(0.45f, 0.47f, 0.52f));
        drawPart(g_quadMesh, base, glm::vec2(-mx, 0.98f), glm::vec2(0.07f, 0.06f), 0.0f, glm::vec3(0.45f, 0.47f, 0.52f));
    }

    // ===== THREE TOWERS (left, centre, right) =====
    float towerX[3] = { -0.55f, 0.0f, 0.55f };
    for (int t = 0; t < 3; ++t) {
        float tx = towerX[t];

        // body
        drawPart(g_quadMesh, base, glm::vec2(tx, 0.65f), glm::vec2(0.16f, 0.40f), 0.0f, glm::vec3(0.40f, 0.42f, 0.48f));
        drawPart(g_quadMesh, base, glm::vec2(tx, 0.65f), glm::vec2(0.12f, 0.38f), 0.0f, glm::vec3(0.50f, 0.52f, 0.58f));

        // windows (2 rows of 2, centred on the tower)
        for (int w = 0; w < 2; ++w) {
            float wy = 0.74f - w * 0.12f;
            drawPart(g_quadMesh, base, glm::vec2(tx - 0.035f, wy), glm::vec2(0.018f, 0.04f), 0.0f, glm::vec3(0.08f, 0.15f, 0.25f));
            drawPart(g_quadMesh, base, glm::vec2(tx + 0.035f, wy), glm::vec2(0.018f, 0.04f), 0.0f, glm::vec3(0.08f, 0.15f, 0.25f));
        }

        // roof
        drawPart(g_triangleMesh, base, glm::vec2(tx, 0.97f), glm::vec2(0.18f, 0.12f), 0.0f, glm::vec3(0.60f, 0.20f, 0.18f));
    }

    // ===== BANNERS AND TREES (mirrored, each tree sits under a banner) =====
    float decorX[2] = { 0.275f, 0.72f };
    for (int d = 0; d < 2; ++d) {
        for (int side = -1; side <= 1; side += 2) {
            float dx = side * decorX[d];

            // banner on the wall
            drawPart(g_quadMesh, base, glm::vec2(dx, 0.79f), glm::vec2(0.06f, 0.09f), 0.0f, glm::vec3(0.15f, 0.35f, 0.85f));
            drawPart(g_triangleMesh, base, glm::vec2(dx, 0.725f), glm::vec2(0.06f, 0.05f), 180.0f, glm::vec3(0.15f, 0.35f, 0.85f));
            drawPart(g_quadMesh, base, glm::vec2(dx, 0.775f), glm::vec2(0.016f, 0.08f), 0.0f, glm::vec3(0.95f, 0.80f, 0.15f));

            // tree (dark green pine: trunk + 3 stacked tiers)
            drawPart(g_quadMesh, base, glm::vec2(dx, 0.550f), glm::vec2(0.022f, 0.060f), 0.0f, glm::vec3(0.40f, 0.26f, 0.12f));
            drawPart(g_triangleMesh, base, glm::vec2(dx, 0.605f), glm::vec2(0.120f, 0.070f), 0.0f, glm::vec3(0.03f, 0.20f, 0.08f));
            drawPart(g_triangleMesh, base, glm::vec2(dx, 0.648f), glm::vec2(0.100f, 0.065f), 0.0f, glm::vec3(0.05f, 0.26f, 0.10f));
            drawPart(g_triangleMesh, base, glm::vec2(dx, 0.690f), glm::vec2(0.075f, 0.060f), 0.0f, glm::vec3(0.07f, 0.33f, 0.13f));
        }
    }
}

inline void drawPlayerModel(PlayerType type, glm::vec2 pos, float time, float redTint, bool jumping, bool ducking, int gray = 0)
{
    float yOffset = jumping ? 0.075f : (ducking ? -0.035f : 0.0f);
    float yScale = ducking ? 0.62f : 1.0f;

    glm::mat4 base = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y + yOffset, 0.0f));
    base = glm::scale(base, glm::vec3(1.0f, yScale, 1.0f));

    float walkBob = std::sin(time * 8.0f) * 0.01f;

    if (type == PLAYER_MELEE) {
        drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.03f), glm::vec2(0.08f, 0.05f), 0.0f, glm::vec3(0.6f, 0.6f, 0.6f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, -0.02f), glm::vec2(0.03f, 0.04f), 0.0f, glm::vec3(0.7f, 0.7f, 0.7f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(-0.03f, -0.06f), glm::vec2(0.01f, 0.03f), walkBob * 10.0f, glm::vec3(0.4f, 0.4f, 0.4f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.03f, -0.06f), glm::vec2(0.01f, 0.03f), -walkBob * 10.0f, glm::vec3(0.4f, 0.4f, 0.4f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.02f), glm::vec2(0.04f, 0.06f), 0.0f, glm::vec3(0.7f, 0.7f, 0.75f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.07f), glm::vec2(0.03f, 0.03f), 0.0f, glm::vec3(0.8f, 0.8f, 0.85f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.065f), glm::vec2(0.02f, 0.005f), 0.0f, glm::vec3(0.2f, 0.2f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, 0.06f), glm::vec2(0.005f, 0.15f), 0.0f, glm::vec3(0.4f, 0.3f, 0.1f), redTint, 0.0f, gray);
        drawPart(g_triangleMesh, base, glm::vec2(0.04f, 0.13f), glm::vec2(0.01f, 0.02f), 0.0f, glm::vec3(0.9f, 0.9f, 0.9f), redTint, 0.0f, gray);
    }
    else if (type == PLAYER_RANGED) {
        drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.03f), glm::vec2(0.08f, 0.05f), 0.0f, glm::vec3(0.5f, 0.3f, 0.1f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, -0.02f), glm::vec2(0.03f, 0.04f), 0.0f, glm::vec3(0.6f, 0.4f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.045f, 0.08f), glm::vec2(0.005f, 0.03f), 20.0f, glm::vec3(0.7f, 0.6f, 0.4f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.045f, 0.08f), glm::vec2(0.005f, 0.03f), -20.0f, glm::vec3(0.7f, 0.6f, 0.4f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.02f), glm::vec2(0.04f, 0.06f), 0.0f, glm::vec3(0.2f, 0.5f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_circleMesh, base, glm::vec2(0.0f, 0.07f), glm::vec2(0.03f, 0.03f), 0.0f, glm::vec3(0.3f, 0.4f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, 0.06f), glm::vec2(0.005f, 0.12f), 0.0f, glm::vec3(0.4f, 0.3f, 0.1f), redTint, 0.0f, gray);
        drawPart(g_circleMesh, base, glm::vec2(0.04f, 0.13f), glm::vec2(0.02f, 0.02f), 0.0f, glm::vec3(0.8f, 0.2f, 0.9f), redTint, 0.0f, gray);
    }
    else if (type == PLAYER_DEFENSE) {
        drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.03f), glm::vec2(0.09f, 0.06f), 0.0f, glm::vec3(0.4f, 0.4f, 0.5f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, -0.02f), glm::vec2(0.03f, 0.05f), 0.0f, glm::vec3(0.5f, 0.5f, 0.6f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.02f), glm::vec2(0.05f, 0.07f), 0.0f, glm::vec3(0.2f, 0.3f, 0.7f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.08f), glm::vec2(0.035f, 0.03f), 0.0f, glm::vec3(0.9f, 0.8f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_triangleMesh, base, glm::vec2(-0.01f, 0.10f), glm::vec2(0.01f, 0.02f), 0.0f, glm::vec3(0.9f, 0.8f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_triangleMesh, base, glm::vec2(0.01f, 0.10f), glm::vec2(0.01f, 0.02f), 0.0f, glm::vec3(0.9f, 0.8f, 0.2f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, 0.06f), glm::vec2(0.005f, 0.12f), 0.0f, glm::vec3(0.9f, 0.9f, 0.9f), redTint, 0.0f, gray);
        drawPart(g_quadMesh, base, glm::vec2(0.04f, 0.05f), glm::vec2(0.02f, 0.005f), 0.0f, glm::vec3(0.9f, 0.8f, 0.2f), redTint, 0.0f, gray);
    }
}

inline void drawEnemyModel(EnemyType type, glm::vec2 pos, float time, float redTint)
{
    glm::mat4 base = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f));
    float walkBob = std::sin(time * 9.0f) * 0.01f;

    if (type == ENEMY_EASY_UPPER) {
        float hoverY = 0.035f + std::sin(time * 5.0f) * 0.015f;
        glm::mat4 flyMat = glm::translate(base, glm::vec3(0.0f, hoverY, 0.0f));
        float wingFlap = std::sin(time * 12.0f) * 22.0f;
        drawPart(g_triangleMesh, flyMat, glm::vec2(0.02f, 0.03f), glm::vec2(0.07f, 0.06f), -30.0f + wingFlap, glm::vec3(0.45f, 0.12f, 0.55f), redTint);
        drawPart(g_triangleMesh, flyMat, glm::vec2(-0.02f, 0.03f), glm::vec2(0.07f, 0.06f), 30.0f - wingFlap, glm::vec3(0.45f, 0.12f, 0.55f), redTint);
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
        drawPart(g_triangleMesh, base, glm::vec2(-0.04f, 0.05f), glm::vec2(0.02f, 0.02f), 0.0f, glm::vec3(0.3f, 0.1f, 0.1f), redTint);
        drawPart(g_triangleMesh, base, glm::vec2(0.04f, 0.05f), glm::vec2(0.02f, 0.02f), 0.0f, glm::vec3(0.3f, 0.1f, 0.1f), redTint);
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