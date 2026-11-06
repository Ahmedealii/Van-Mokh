#pragma once
#include "Models.h"
#include "GameLogic.h"
#include <algorithm>
#include <cmath>
#include <string>

// =========================================================================
// DMET 502 Computer Graphics - Castle Siege Tower Defense
// Student Module: UIAndCutscene.h (Dev Youssef)
// Procedural Student-Level C++ / Modern OpenGL Core Profile (NDC Layout)
// =========================================================================

// Smoothstep interpolation helper
inline float smoothstep(float edge0, float edge1, float x) {
    float t = (x - edge0) / (edge1 - edge0);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return t * t * (3.0f - 2.0f * t);
}

// Linear interpolation helper
inline float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Axis-aligned bounding box intersection test in NDC space
inline bool isPointInBox(glm::vec2 pt, glm::vec2 center, glm::vec2 size)
{
    return (pt.x >= center.x - size.x * 0.5f && pt.x <= center.x + size.x * 0.5f &&
        pt.y >= center.y - size.y * 0.5f && pt.y <= center.y + size.y * 0.5f);
}

// -------------------------------------------------------------------------
// Procedural Geometric Typography (No external font loaders / No FreeType)
// Strictly follows classic index loops: for (int i = 0; i < (int)vec.size(); ++i)
// -------------------------------------------------------------------------
inline void drawProceduralChar(char c, glm::vec2 pos, float scale, glm::vec3 col)
{
    glm::mat4 base(1.0f);
    if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';

    float w = 0.020f * scale;
    float h = 0.034f * scale;
    float t = 0.004f * scale;

    float xl = pos.x;
    float xm = pos.x + w * 0.5f;
    float xr = pos.x + w;
    float yb = pos.y;
    float ym = pos.y + h * 0.5f;
    float yt = pos.y + h;

    if (c == 'A') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'B') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, yb + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
    }
    else if (c == 'C') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'D') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.45f, yt), glm::vec2(w * 0.9f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.45f, yb), glm::vec2(w * 0.9f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h - t), 0.0f, col);
    }
    else if (c == 'E') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.35f, ym), glm::vec2(w * 0.7f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'F') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.35f, ym), glm::vec2(w * 0.7f, t), 0.0f, col);
    }
    else if (c == 'G') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, yb + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm + w * 0.2f, ym), glm::vec2(w * 0.6f, t), 0.0f, col);
    }
    else if (c == 'H') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'I') {
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w * 0.7f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w * 0.7f, t), 0.0f, col);
    }
    else if (c == 'J') {
        drawPart(g_quadMesh, base, glm::vec2(xr, yb + h * 0.55f), glm::vec2(t, h * 0.9f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl, yb + h * 0.2f), glm::vec2(t, h * 0.4f), 0.0f, col);
    }
    else if (c == 'K') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm + w * 0.1f, ym + h * 0.25f), glm::vec2(t, h * 0.58f), -38.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm + w * 0.1f, ym - h * 0.25f), glm::vec2(t, h * 0.58f), 38.0f, col);
    }
    else if (c == 'L') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'M') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.25f, ym + h * 0.15f), glm::vec2(t, h * 0.62f), 28.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr - w * 0.25f, ym + h * 0.15f), glm::vec2(t, h * 0.62f), -28.0f, col);
    }
    else if (c == 'N') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(t, h * 1.15f), 26.0f, col);
    }
    else if (c == 'O') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'P') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
    }
    else if (c == 'Q') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr + w * 0.08f, yb - h * 0.06f), glm::vec2(t, h * 0.35f), -42.0f, col);
    }
    else if (c == 'R') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm + w * 0.15f, yb + h * 0.22f), glm::vec2(t, h * 0.55f), 35.0f, col);
    }
    else if (c == 'S') {
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl, ym + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, yb + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
    }
    else if (c == 'T') {
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(t, h), 0.0f, col);
    }
    else if (c == 'U') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'V') {
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.25f, ym), glm::vec2(t, h * 1.08f), 17.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr - w * 0.25f, ym), glm::vec2(t, h * 1.08f), -17.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb + t * 0.5f), glm::vec2(t * 1.5f, t), 0.0f, col);
    }
    else if (c == 'W') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb + h * 0.35f), glm::vec2(t, h * 0.7f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
    }
    else if (c == 'X') {
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(t, h * 1.2f), 32.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(t, h * 1.2f), -32.0f, col);
    }
    else if (c == 'Y') {
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.25f, ym + h * 0.25f), glm::vec2(t, h * 0.58f), 28.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr - w * 0.25f, ym + h * 0.25f), glm::vec2(t, h * 0.58f), -28.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb + h * 0.25f), glm::vec2(t, h * 0.5f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(t * 1.2f, t * 1.2f), 0.0f, col);
    }
    else if (c == 'Z') {
        drawPart(g_quadMesh, base, glm::vec2(xm, yt), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb), glm::vec2(w, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(t, h * 1.2f), 35.0f, col);
    }
    else if (c == '!') {
        drawPart(g_quadMesh, base, glm::vec2(xm, ym + h * 0.18f), glm::vec2(t, h * 0.65f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, yb + t * 0.8f), glm::vec2(t * 1.4f, t * 1.4f), 0.0f, col);
    }
    else if (c == ':') {
        drawPart(g_quadMesh, base, glm::vec2(xm, ym + h * 0.22f), glm::vec2(t * 1.3f, t * 1.3f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm, ym - h * 0.22f), glm::vec2(t * 1.3f, t * 1.3f), 0.0f, col);
    }
    else if (c == '-') {
        drawPart(g_quadMesh, base, glm::vec2(xm, ym), glm::vec2(w * 0.8f, t), 0.0f, col);
    }
    else if (c == '.') {
        drawPart(g_quadMesh, base, glm::vec2(xm, yb + t * 0.8f), glm::vec2(t * 1.4f, t * 1.4f), 0.0f, col);
    }
    else if (c == ',') {
        drawPart(g_quadMesh, base, glm::vec2(xm, yb + t * 0.8f), glm::vec2(t * 1.4f, t * 1.4f), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xm - t * 0.3f, yb - t * 0.8f), glm::vec2(t, t * 2.0f), -25.0f, col);
    }
    else if (c == '[') {
        drawPart(g_quadMesh, base, glm::vec2(xl, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.3f, yt), glm::vec2(w * 0.6f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.3f, yb), glm::vec2(w * 0.6f, t), 0.0f, col);
    }
    else if (c == ']') {
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr - w * 0.3f, yt), glm::vec2(w * 0.6f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr - w * 0.3f, yb), glm::vec2(w * 0.6f, t), 0.0f, col);
    }
    else if (c >= '0' && c <= '9') {
        drawDigit(c - '0', glm::vec2(xm, ym), scale * 0.77f, col);
        }
}

inline void drawText(const std::string& text, glm::vec2 pos, float scale, glm::vec3 col)
{
    float charSpacing = 0.026f * scale;
    for (int i = 0; i < (int)text.length(); ++i) {
        char c = text[i];
        if (c != ' ') {
            drawProceduralChar(c, pos + glm::vec2((float)i * charSpacing, 0.0f), scale, col);
        }
    }
}

inline void drawText(const std::string& text, glm::vec2 pos, glm::vec3 col)
{
    drawText(text, pos, 1.0f, col);
}

// Automatically centers procedural text around a center coordinate (X, Y)
inline void drawCenteredText(const std::string& text, glm::vec2 center, float scale, glm::vec3 col)
{
    float charSpacing = 0.026f * scale;
    float totalW = (float)text.length() * charSpacing;
    glm::vec2 startPos = center - glm::vec2(totalW * 0.5f - 0.010f * scale, 0.017f * scale);
    drawText(text, startPos, scale, col);
}

// Centered procedural text with a rich 3D drop-shadow
inline void drawCenteredTextWithShadow(const std::string& text, glm::vec2 center, float scale, glm::vec3 col, glm::vec3 shadowCol = glm::vec3(0.04f, 0.05f, 0.07f))
{
    float offset = 0.003f * scale;
    drawCenteredText(text, center + glm::vec2(offset, -offset), scale, shadowCol);
    drawCenteredText(text, center, scale, col);
}

// -------------------------------------------------------------------------
// Static HUD Gold Coin for the Coin Counter Widget (Zero motion, stays completely still)
// -------------------------------------------------------------------------
inline void drawStaticHUDCoin(glm::vec2 pos)
{
    glm::mat4 base(1.0f);
    // Outer drop shadow
    drawPart(g_circleMesh, base, pos + glm::vec2(0.002f, -0.003f), glm::vec2(0.034f, 0.034f), 0.0f, glm::vec3(0.06f, 0.05f, 0.03f));
    // Outer burnished gold rim
    drawPart(g_circleMesh, base, pos, glm::vec2(0.032f, 0.032f), 0.0f, glm::vec3(0.85f, 0.65f, 0.12f));
    // Main coin face
    drawPart(g_circleMesh, base, pos, glm::vec2(0.027f, 0.027f), 0.0f, glm::vec3(1.00f, 0.84f, 0.18f));
    // Inner bevel / reflective ring
    drawPart(g_circleMesh, base, pos, glm::vec2(0.020f, 0.020f), 0.0f, glm::vec3(1.00f, 0.94f, 0.45f));
    // Center emblem bar
    drawPart(g_quadMesh, base, pos, glm::vec2(0.006f, 0.019f), 0.0f, glm::vec3(0.85f, 0.62f, 0.08f));
    // Center emblem dot
    drawPart(g_circleMesh, base, pos, glm::vec2(0.007f, 0.007f), 0.0f, glm::vec3(1.00f, 0.88f, 0.20f));
}

// -------------------------------------------------------------------------
// Collectible Gold Coin Animation
// Pulses scale at sin(time * 6.0f) dropped by defeated enemies in battle
// -------------------------------------------------------------------------
inline void drawCollectibleCoin(glm::vec2 pos, float time)
{
    glm::mat4 base(1.0f);
    float pulse = 1.0f + 0.12f * std::sin(time * 6.0f);
    float floatBob = 0.008f * std::sin(time * 4.0f);
    glm::vec2 cpos = pos + glm::vec2(0.0f, floatBob);

    // Outer aura / glow
    drawPart(g_circleMesh, base, cpos, glm::vec2(0.038f * pulse, 0.038f * pulse), 0.0f, glm::vec3(0.95f, 0.72f, 0.10f));
    // Main coin disc
    drawPart(g_circleMesh, base, cpos, glm::vec2(0.029f * pulse, 0.029f * pulse), 0.0f, glm::vec3(1.00f, 0.84f, 0.05f));
    // Inner reflective rim
    drawPart(g_circleMesh, base, cpos, glm::vec2(0.021f * pulse, 0.021f * pulse), 0.0f, glm::vec3(1.00f, 0.94f, 0.40f));
    // Center emblem bar
    drawPart(g_quadMesh, base, cpos, glm::vec2(0.007f * pulse, 0.020f * pulse), 0.0f, glm::vec3(0.85f, 0.62f, 0.05f));
}

// -------------------------------------------------------------------------
// Visual Effects & Projectiles
// Straight player arrows vs enemy high/low magic orbs + hit explosion animation
// -------------------------------------------------------------------------
inline void drawProjectile(const Projectile& p, float time)
{
    glm::mat4 base(1.0f);
    float lane_center_y[4] = { 0.375f, 0.025f, -0.325f, -0.675f };
    float y = lane_center_y[p.lane];
    if (!p.fromPlayer) {
        y += p.isHigh ? 0.055f : -0.055f;
    }

    // Hit-impact particle explosion
    if (p.hitting) {
        float prog = (p.hitAnimTimer > 0.0f && p.hitAnimTimer <= 0.20f) ? (1.0f - p.hitAnimTimer / 0.20f) : 1.0f;
        float expand = prog * 0.055f;
        float alpha = 1.0f - prog * 0.85f;

        glm::vec3 coreCol = p.fromPlayer ? glm::vec3(1.0f, 0.90f, 0.30f) : glm::vec3(1.0f, 0.45f, 0.15f);
        drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.032f * (1.0f + prog * 1.8f), 0.032f * (1.0f + prog * 1.8f)), 0.0f, coreCol * alpha);
        drawPart(g_triangleMesh, base, glm::vec2(p.x, y), glm::vec2(0.040f * (1.0f + prog * 1.5f), 0.040f * (1.0f + prog * 1.5f)), time * 450.0f, glm::vec3(1.0f, 0.30f, 0.10f) * alpha);

        // Debris / spark particles expanding radially
        drawPart(g_quadMesh, base, glm::vec2(p.x + expand, y + expand), glm::vec2(0.010f * alpha, 0.010f * alpha), 45.0f, glm::vec3(1.0f, 0.85f, 0.20f));
        drawPart(g_quadMesh, base, glm::vec2(p.x - expand, y + expand), glm::vec2(0.010f * alpha, 0.010f * alpha), 45.0f, glm::vec3(1.0f, 0.85f, 0.20f));
        drawPart(g_quadMesh, base, glm::vec2(p.x + expand, y - expand), glm::vec2(0.010f * alpha, 0.010f * alpha), 45.0f, glm::vec3(1.0f, 0.85f, 0.20f));
        drawPart(g_quadMesh, base, glm::vec2(p.x - expand, y - expand), glm::vec2(0.010f * alpha, 0.010f * alpha), 45.0f, glm::vec3(1.0f, 0.85f, 0.20f));
        return;
    }

    if (p.fromPlayer) {
        // Player Straight Arrow (Shaft, Sharp Steel Tip, Tail Fletching)
        drawPart(g_quadMesh, base, glm::vec2(p.x, y), glm::vec2(0.048f, 0.007f), 0.0f, glm::vec3(0.65f, 0.42f, 0.18f));
        drawPart(g_triangleMesh, base, glm::vec2(p.x + 0.026f, y), glm::vec2(0.020f, 0.022f), -90.0f, glm::vec3(0.85f, 0.88f, 0.94f));
        drawPart(g_quadMesh, base, glm::vec2(p.x - 0.022f, y), glm::vec2(0.012f, 0.016f), 0.0f, glm::vec3(0.95f, 0.95f, 0.95f));
    }
    else {
        if (p.isHigh) {
            // Enemy High Magic Orb (Pulsing violet/arcane sphere with centered core)
            float orbPulse = 1.0f + 0.12f * std::sin(time * 12.0f);
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.024f * orbPulse, 0.024f * orbPulse), 0.0f, glm::vec3(0.70f, 0.15f, 0.85f));
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.016f, 0.016f), 0.0f, glm::vec3(0.98f, 0.40f, 0.95f));
            drawPart(g_triangleMesh, base, glm::vec2(p.x, y), glm::vec2(0.013f, 0.013f), time * 320.0f, glm::vec3(1.0f, 1.0f, 1.0f));
        }
        else {
            // Enemy Low Magic Orb (Fiery orange/red fireball with centered core)
            float firePulse = 1.0f + 0.10f * std::sin(time * 10.0f);
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.024f * firePulse, 0.024f * firePulse), 0.0f, glm::vec3(1.0f, 0.35f, 0.05f));
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.015f, 0.015f), 0.0f, glm::vec3(1.0f, 0.88f, 0.20f));
            drawPart(g_triangleMesh, base, glm::vec2(p.x, y), glm::vec2(0.013f, 0.013f), -time * 320.0f, glm::vec3(1.0f, 0.95f, 0.40f));
        }
    }
}

// -------------------------------------------------------------------------
// Custom Geometric 3D Beveled Button System & Hit Testing
// High tactile depth: drop shadows, metallic rim, light bevels, clean icons
// labelCode 1: Play/Resume (Sharp Triangle + "PLAY" or "RESUME")
// labelCode 2: Retry/Play Again (Sharp Loop Arrow + "RETRY")
// labelCode 3: Exit/Main Menu (Bold Solid Cross + "EXIT")
// -------------------------------------------------------------------------
inline void drawButton(glm::vec2 center, glm::vec2 size, glm::vec3 color, int labelCode, glm::vec2 cursorNDC, const std::string& customLabel = "")
{
    glm::mat4 base(1.0f);
    bool hovered = isPointInBox(cursorNDC, center, size);
    float highlight = hovered ? 0.45f : 0.0f;
    glm::vec2 currentSize = hovered ? size * 1.035f : size;

    // 1. Soft dark drop shadow
    drawPart(g_quadMesh, base, center + glm::vec2(0.006f, -0.007f), currentSize + glm::vec2(0.012f, 0.012f), 0.0f, glm::vec3(0.03f, 0.04f, 0.06f));

    // 2. Chiseled metallic rim / border (glowing gold on hover)
    glm::vec3 borderCol = hovered ? glm::vec3(1.0f, 0.88f, 0.32f) : glm::vec3(0.38f, 0.42f, 0.50f);
    drawPart(g_quadMesh, base, center, currentSize + glm::vec2(0.010f, 0.010f), 0.0f, borderCol, 0.0f, highlight);

    // 3. Main button face (brightened on hover)
    glm::vec3 faceCol = hovered ? color * 1.15f : color;
    drawPart(g_quadMesh, base, center, currentSize, 0.0f, faceCol, 0.0f, hovered ? 0.12f : 0.0f);

    // 4. Tactile 3D Bevel strips (top highlight, bottom shadow)
    float bevelH = 0.007f;
    drawPart(g_quadMesh, base, center + glm::vec2(0.0f, currentSize.y * 0.5f - bevelH * 0.5f), glm::vec2(currentSize.x, bevelH), 0.0f, faceCol * 1.35f);
    drawPart(g_quadMesh, base, center - glm::vec2(0.0f, currentSize.y * 0.5f - bevelH * 0.5f), glm::vec2(currentSize.x, bevelH), 0.0f, faceCol * 0.62f);

    glm::vec3 textCol(1.0f, 1.0f, 1.0f);

    std::string text = customLabel;
    if (text.empty()) {
        if (labelCode == 1) {
            text = (g_gameState == STATE_PAUSED) ? "RESUME" : "PLAY";
        }
        else if (labelCode == 2) {
            text = "RETRY";
        }
        else if (labelCode == 3) {
            text = "EXIT";
        }
    }

    float textScale = (size.y > 0.13f) ? 1.12f : ((size.y < 0.12f) ? 0.98f : 1.05f);

    // Clean, centered text with shadow (no logos/icons)
    drawCenteredTextWithShadow(text, center, textScale, textCol, glm::vec3(0.05f, 0.06f, 0.08f));
}

// -------------------------------------------------------------------------
// Reusable Medieval Modal Window Frame
// Compact dimensions with calibrated vertical padding to prevent large gaps
// crestCode: 1 = Royal Crown & Swords (Main Menu)
//            2 = Skull & Broken Shield (Defeat)
//            3 = Golden Chalice & Laurels (Victory)
//            4 = Portcullis & Crystal (Pause)
// -------------------------------------------------------------------------
inline void drawModalFrame(glm::vec2 center, glm::vec2 size, glm::vec3 themeColor, const std::string& title, const std::string& subtitle, int crestCode, float time)
{
    glm::mat4 base(1.0f);

    // 1. Heavy drop shadow
    drawPart(g_quadMesh, base, center + glm::vec2(0.012f, -0.014f), size + glm::vec2(0.020f, 0.020f), 0.0f, glm::vec3(0.03f, 0.04f, 0.06f));

    // 2. Chiseled stone masonry outer border
    drawPart(g_quadMesh, base, center, size + glm::vec2(0.018f, 0.018f), 0.0f, glm::vec3(0.22f, 0.25f, 0.32f));

    // 3. Ornate gold / bronze filigree rim
    drawPart(g_quadMesh, base, center, size + glm::vec2(0.008f, 0.008f), 0.0f, glm::vec3(0.75f, 0.60f, 0.22f));

    // 4. Main theme backdrop
    drawPart(g_quadMesh, base, center, size, 0.0f, themeColor);

    // 5. Inset dark velvet surface
    drawPart(g_quadMesh, base, center, size - glm::vec2(0.020f, 0.020f), 0.0f, glm::vec3(0.10f, 0.12f, 0.16f));

    // 6. Corner decorative studs
    float cx = size.x * 0.5f - 0.020f;
    float cy = size.y * 0.5f - 0.020f;
    glm::vec3 studCol(0.85f, 0.70f, 0.25f);
    drawPart(g_circleMesh, base, center + glm::vec2(-cx, cy), glm::vec2(0.010f, 0.010f), 0.0f, studCol);
    drawPart(g_circleMesh, base, center + glm::vec2(cx, cy), glm::vec2(0.010f, 0.010f), 0.0f, studCol);
    drawPart(g_circleMesh, base, center + glm::vec2(-cx, -cy), glm::vec2(0.010f, 0.010f), 0.0f, studCol);
    drawPart(g_circleMesh, base, center + glm::vec2(cx, -cy), glm::vec2(0.010f, 0.010f), 0.0f, studCol);

    // 7. Header Ribbon Banner
    float bannerY = center.y + size.y * 0.5f - 0.125f;
    float bannerW = size.x - 0.10f;
    float bannerH = 0.080f;

    // Outer gold trim for banner
    drawPart(g_quadMesh, base, glm::vec2(center.x, bannerY), glm::vec2(bannerW + 0.008f, bannerH + 0.008f), 0.0f, glm::vec3(0.85f, 0.72f, 0.25f));
    // Inner banner surface
    drawPart(g_quadMesh, base, glm::vec2(center.x, bannerY), glm::vec2(bannerW, bannerH), 0.0f, themeColor * 0.85f);

    // 8. Procedural Crest Icon above the banner
    float crestY = bannerY + 0.072f;
    if (crestCode == 1) {
        float bob = std::sin(time * 3.0f) * 0.003f;
        glm::vec2 crownPos = glm::vec2(center.x, crestY + bob);
        drawPart(g_quadMesh, base, crownPos, glm::vec2(0.07f, 0.016f), 0.0f, glm::vec3(0.95f, 0.80f, 0.20f));
        drawPart(g_triangleMesh, base, crownPos + glm::vec2(-0.024f, 0.018f), glm::vec2(0.020f, 0.022f), 0.0f, glm::vec3(0.95f, 0.80f, 0.20f));
        drawPart(g_triangleMesh, base, crownPos + glm::vec2(0.0f, 0.022f), glm::vec2(0.024f, 0.028f), 0.0f, glm::vec3(1.0f, 0.90f, 0.30f));
        drawPart(g_triangleMesh, base, crownPos + glm::vec2(0.024f, 0.018f), glm::vec2(0.020f, 0.022f), 0.0f, glm::vec3(0.95f, 0.80f, 0.20f));
        drawPart(g_circleMesh, base, crownPos + glm::vec2(0.0f, 0.032f), glm::vec2(0.005f, 0.005f), 0.0f, glm::vec3(0.9f, 0.2f, 0.2f));
    }
    else if (crestCode == 2) {
        glm::vec2 shieldPos = glm::vec2(center.x, crestY);
        drawPart(g_quadMesh, base, shieldPos, glm::vec2(0.056f, 0.042f), 0.0f, glm::vec3(0.55f, 0.15f, 0.15f));
        drawPart(g_triangleMesh, base, shieldPos - glm::vec2(0.0f, 0.028f), glm::vec2(0.056f, 0.026f), 180.0f, glm::vec3(0.55f, 0.15f, 0.15f));
        drawPart(g_quadMesh, base, shieldPos, glm::vec2(0.005f, 0.045f), 35.0f, glm::vec3(0.1f, 0.1f, 0.1f));
        drawPart(g_circleMesh, base, shieldPos + glm::vec2(0.0f, 0.004f), glm::vec2(0.013f, 0.013f), 0.0f, glm::vec3(0.85f, 0.85f, 0.88f));
    }
    else if (crestCode == 3) {
        float glow = 1.0f + 0.08f * std::sin(time * 6.0f);
        glm::vec2 cupPos = glm::vec2(center.x, crestY);
        drawPart(g_quadMesh, base, cupPos - glm::vec2(0.0f, 0.020f), glm::vec2(0.040f, 0.008f), 0.0f, glm::vec3(0.95f, 0.80f, 0.20f));
        drawPart(g_quadMesh, base, cupPos - glm::vec2(0.0f, 0.010f), glm::vec2(0.012f, 0.016f), 0.0f, glm::vec3(0.95f, 0.80f, 0.20f));
        drawPart(g_quadMesh, base, cupPos + glm::vec2(0.0f, 0.010f), glm::vec2(0.050f * glow, 0.028f), 0.0f, glm::vec3(1.0f, 0.90f, 0.30f));
        drawPart(g_circleMesh, base, cupPos + glm::vec2(0.0f, 0.032f), glm::vec2(0.006f, 0.006f), 0.0f, glm::vec3(1.0f, 1.0f, 0.8f));
    }
    else if (crestCode == 4) {
        glm::vec2 pausePos = glm::vec2(center.x, crestY);
        drawPart(g_quadMesh, base, pausePos - glm::vec2(0.012f, 0.0f), glm::vec2(0.009f, 0.030f), 0.0f, glm::vec3(0.35f, 0.75f, 1.0f));
        drawPart(g_quadMesh, base, pausePos + glm::vec2(0.012f, 0.0f), glm::vec2(0.009f, 0.030f), 0.0f, glm::vec3(0.35f, 0.75f, 1.0f));
    }

    // 9. Centered Title Text with Drop Shadow
    glm::vec3 titleCol = (crestCode == 2) ? glm::vec3(1.0f, 0.40f, 0.40f) : glm::vec3(1.0f, 0.90f, 0.35f);
    drawCenteredTextWithShadow(title, glm::vec2(center.x, bannerY), 1.12f, titleCol);

    // 10. Centered Subtitle Text (positioned cleanly below the banner box)
    drawCenteredText(subtitle, glm::vec2(center.x, bannerY - 0.068f), 0.65f, glm::vec3(0.75f, 0.80f, 0.90f));

    // 11. Ornate divider rule with diamond jewel
    float divY = bannerY - 0.098f;
    drawPart(g_quadMesh, base, glm::vec2(center.x, divY), glm::vec2(size.x - 0.16f, 0.004f), 0.0f, glm::vec3(0.75f, 0.62f, 0.25f));
    drawPart(g_quadMesh, base, glm::vec2(center.x, divY), glm::vec2(0.010f, 0.010f), 45.0f, glm::vec3(0.95f, 0.85f, 0.30f));
}

// -------------------------------------------------------------------------
// Enhanced Castle Tower & Fortified Gateways (Self-Contained in UIAndCutscene.h)
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// Forward Declaration
// -------------------------------------------------------------------------
inline void drawDetailedBottomEmbankment();

// -------------------------------------------------------------------------
// Enhanced Castle Tower & Fortified Gateways (Self-Contained in UIAndCutscene.h)
// -------------------------------------------------------------------------
inline void drawDetailedHomeBase()
{
    glm::mat4 base(1.0f);

    // 1. Heavy drop shadow behind the tower
    drawPart(g_quadMesh, base, glm::vec2(-0.89f, -0.15f), glm::vec2(0.20f, 1.48f), 0.0f, glm::vec3(0.10f, 0.12f, 0.15f));

    // 2. Reinforced foundation stone plinth at base
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, -0.80f), glm::vec2(0.21f, 0.22f), 0.0f, glm::vec3(0.35f, 0.37f, 0.42f));
    drawPart(g_quadMesh, base, glm::vec2(-0.89f, -0.70f), glm::vec2(0.19f, 0.015f), 0.0f, glm::vec3(0.28f, 0.30f, 0.35f));

    // 3. Main tower body
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, -0.15f), glm::vec2(0.18f, 1.45f), 0.0f, glm::vec3(0.45f, 0.47f, 0.52f));
    drawPart(g_quadMesh, base, glm::vec2(-0.88f, -0.15f), glm::vec2(0.13f, 1.40f), 0.0f, glm::vec3(0.55f, 0.57f, 0.62f));

    // 4. Detailed Ashlar Masonry: horizontal mortar lines & brick highlights
    for (int r = 0; r < 18; ++r) {
        float by = -0.78f + (float)r * 0.076f;
        drawPart(g_quadMesh, base, glm::vec2(-0.88f, by), glm::vec2(0.13f, 0.004f), 0.0f, glm::vec3(0.34f, 0.36f, 0.40f));
        float bx1 = (r % 2 == 0) ? -0.92f : -0.86f;
        float bx2 = (r % 2 == 0) ? -0.85f : -0.91f;
        drawPart(g_quadMesh, base, glm::vec2(bx1, by + 0.038f), glm::vec2(0.004f, 0.072f), 0.0f, glm::vec3(0.34f, 0.36f, 0.40f));
        if (r % 3 == 0) {
            drawPart(g_quadMesh, base, glm::vec2(bx2, by + 0.038f), glm::vec2(0.040f, 0.055f), 0.0f, glm::vec3(0.62f, 0.64f, 0.70f));
        }
    }

    // 5. Four Architectural Lane Gateways / Doors (One per lane)
    float cy_arr[4] = { 0.375f, 0.025f, -0.325f, -0.675f };
    for (int i = 0; i < 4; ++i) {
        float cy = cy_arr[i];

        // Heavy stone arch frame
        drawPart(g_quadMesh, base, glm::vec2(-0.835f, cy - 0.015f), glm::vec2(0.074f, 0.170f), 0.0f, glm::vec3(0.32f, 0.34f, 0.38f));
        drawPart(g_circleMesh, base, glm::vec2(-0.835f, cy + 0.065f), glm::vec2(0.038f, 0.038f), 0.0f, glm::vec3(0.32f, 0.34f, 0.38f));

        // Keystone atop each arch
        drawPart(g_quadMesh, base, glm::vec2(-0.835f, cy + 0.100f), glm::vec2(0.018f, 0.016f), 0.0f, glm::vec3(0.70f, 0.72f, 0.78f));

        // Deep interior archway with warm amber glow
        drawPart(g_quadMesh, base, glm::vec2(-0.835f, cy - 0.015f), glm::vec2(0.056f, 0.155f), 0.0f, glm::vec3(0.24f, 0.15f, 0.08f));
        drawPart(g_circleMesh, base, glm::vec2(-0.835f, cy + 0.062f), glm::vec2(0.028f, 0.028f), 0.0f, glm::vec3(0.24f, 0.15f, 0.08f));

        // Heavy iron-reinforced wooden door planks
        drawPart(g_quadMesh, base, glm::vec2(-0.835f, cy - 0.015f), glm::vec2(0.048f, 0.145f), 0.0f, glm::vec3(0.42f, 0.26f, 0.14f));
        drawPart(g_circleMesh, base, glm::vec2(-0.835f, cy + 0.058f), glm::vec2(0.024f, 0.024f), 0.0f, glm::vec3(0.42f, 0.26f, 0.14f));

        // Horizontal wrought-iron reinforcement bands
        drawPart(g_quadMesh, base, glm::vec2(-0.835f, cy + 0.025f), glm::vec2(0.050f, 0.012f), 0.0f, glm::vec3(0.20f, 0.22f, 0.26f));
        drawPart(g_quadMesh, base, glm::vec2(-0.835f, cy - 0.055f), glm::vec2(0.050f, 0.012f), 0.0f, glm::vec3(0.20f, 0.22f, 0.26f));

        // Vertical plank seam & golden pull ring
        drawPart(g_quadMesh, base, glm::vec2(-0.835f, cy - 0.015f), glm::vec2(0.005f, 0.145f), 0.0f, glm::vec3(0.18f, 0.12f, 0.06f));
        drawPart(g_circleMesh, base, glm::vec2(-0.822f, cy - 0.015f), glm::vec2(0.008f, 0.008f), 0.0f, glm::vec3(0.88f, 0.72f, 0.22f));
    }

    // 6. Overhanging Machicolation Shelf & Corbels
    drawPart(g_quadMesh, base, glm::vec2(-0.89f, 0.585f), glm::vec2(0.22f, 0.025f), 0.0f, glm::vec3(0.32f, 0.34f, 0.38f));
    for (int i = 0; i < 5; ++i) {
        float kx = -0.97f + (float)i * 0.042f;
        drawPart(g_quadMesh, base, glm::vec2(kx, 0.565f), glm::vec2(0.014f, 0.022f), 0.0f, glm::vec3(0.28f, 0.30f, 0.34f));
    }

    // 7. Fortified Battlements / Crenellations with Arrow Slits
    for (int i = 0; i < 4; ++i) {
        float cx = -0.96f + (float)i * 0.045f;
        drawPart(g_quadMesh, base, glm::vec2(cx, 0.615f), glm::vec2(0.032f, 0.055f), 0.0f, glm::vec3(0.50f, 0.52f, 0.58f));
        drawPart(g_quadMesh, base, glm::vec2(cx, 0.635f), glm::vec2(0.034f, 0.012f), 0.0f, glm::vec3(0.62f, 0.64f, 0.70f));
        drawPart(g_quadMesh, base, glm::vec2(cx, 0.612f), glm::vec2(0.005f, 0.026f), 0.0f, glm::vec3(0.16f, 0.18f, 0.22f));
    }

    // 8. Ornate Conical Royal Blue Castle Roof with Eaves & Shingles
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, 0.645f), glm::vec2(0.22f, 0.014f), 0.0f, glm::vec3(0.82f, 0.68f, 0.24f));
    drawPart(g_triangleMesh, base, glm::vec2(-0.90f, 0.730f), glm::vec2(0.22f, 0.170f), 0.0f, glm::vec3(0.16f, 0.38f, 0.82f));
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, 0.675f), glm::vec2(0.17f, 0.008f), 0.0f, glm::vec3(0.26f, 0.52f, 0.95f));
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, 0.710f), glm::vec2(0.12f, 0.008f), 0.0f, glm::vec3(0.26f, 0.52f, 0.95f));
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, 0.745f), glm::vec2(0.07f, 0.008f), 0.0f, glm::vec3(0.26f, 0.52f, 0.95f));

    // 9. Golden Royal Spire Finial & Pennant Flag at Peak
    drawPart(g_quadMesh, base, glm::vec2(-0.90f, 0.825f), glm::vec2(0.006f, 0.040f), 0.0f, glm::vec3(0.95f, 0.82f, 0.25f));
    drawPart(g_circleMesh, base, glm::vec2(-0.90f, 0.845f), glm::vec2(0.012f, 0.012f), 0.0f, glm::vec3(1.0f, 0.90f, 0.35f));
    drawPart(g_triangleMesh, base, glm::vec2(-0.880f, 0.835f), glm::vec2(0.035f, 0.022f), -90.0f, glm::vec3(0.85f, 0.22f, 0.22f));

    // Foreground bottom woodland embankment & trees (rendered on top of lanes)
    drawDetailedBottomEmbankment();
}

// -------------------------------------------------------------------------
// Foreground Bottom Woodland Embankment, Mossy Boulders & Pine Trees
// Rendered on top of the lane layer along the bottom perimeter (no sticks)
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// Foreground Bottom Woodland Embankment, Natural Boulders & Pine Trees
// Scaled exactly to the width of the battlefield lanes (-0.78f to +0.95f)
// -------------------------------------------------------------------------
inline void drawDetailedBottomEmbankment()
{
    glm::mat4 base(1.0f);

    // 1. Embankment strip scaled exactly to match lanes (center X = 0.085f, width = 1.73f)
    drawPart(g_quadMesh, base, glm::vec2(0.085f, -0.925f), glm::vec2(1.73f, 0.150f), 0.0f, glm::vec3(0.14f, 0.16f, 0.20f));
    drawPart(g_quadMesh, base, glm::vec2(0.085f, -0.850f), glm::vec2(1.73f, 0.006f), 0.0f, glm::vec3(0.18f, 0.20f, 0.25f));

    // 2. Natural gray cobblestones along the embankment (no green spots)
    for (int c = 0; c < 16; ++c) {
        float cx = -0.72f + (float)c * 0.108f;
        float cy = -0.940f + ((c % 3) == 0 ? 0.012f : -0.008f);
        drawPart(g_circleMesh, base, glm::vec2(cx, cy), glm::vec2(0.030f, 0.018f), 0.0f, glm::vec3(0.28f, 0.30f, 0.35f));
        drawPart(g_circleMesh, base, glm::vec2(cx - 0.004f, cy + 0.003f), glm::vec2(0.022f, 0.012f), 0.0f, glm::vec3(0.42f, 0.44f, 0.50f));
    }

    // 3. Multi-Tiered Dense Pine Trees & Natural Gray River Rocks
    // Positioned strictly within the lane span with no green circles on stones
    for (int i = 0; i < 7; ++i) {
        float tx = -0.68f + (float)i * 0.250f;
        // Tree trunk
        drawPart(g_quadMesh, base, glm::vec2(tx, -0.920f), glm::vec2(0.012f, 0.055f), 0.0f, glm::vec3(0.38f, 0.22f, 0.10f));
        // Tree foliage layers overlapping in front of bottom lane (-0.85f)
        drawPart(g_triangleMesh, base, glm::vec2(tx, -0.890f), glm::vec2(0.082f, 0.065f), 0.0f, glm::vec3(0.12f, 0.38f, 0.16f));
        drawPart(g_triangleMesh, base, glm::vec2(tx, -0.860f), glm::vec2(0.068f, 0.058f), 0.0f, glm::vec3(0.18f, 0.48f, 0.22f));
        drawPart(g_triangleMesh, base, glm::vec2(tx, -0.830f), glm::vec2(0.050f, 0.050f), 0.0f, glm::vec3(0.24f, 0.58f, 0.28f));

        // Natural smooth stone boulders (pure rock/granite tones, zero green circles)
        drawPart(g_circleMesh, base, glm::vec2(tx + 0.065f, -0.925f), glm::vec2(0.044f, 0.028f), 0.0f, glm::vec3(0.32f, 0.34f, 0.38f));
        drawPart(g_circleMesh, base, glm::vec2(tx + 0.070f, -0.920f), glm::vec2(0.032f, 0.020f), 0.0f, glm::vec3(0.46f, 0.48f, 0.54f));
        drawPart(g_circleMesh, base, glm::vec2(tx + 0.062f, -0.914f), glm::vec2(0.018f, 0.010f), 0.0f, glm::vec3(0.58f, 0.60f, 0.66f));
    }
}
// -------------------------------------------------------------------------
// Enhanced Castle Wall Background (Self-Contained in UIAndCutscene.h)
// -------------------------------------------------------------------------
inline void drawDetailedEnvironment(float time)
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
}
// -------------------------------------------------------------------------
// Procedural Battlefield Lanes Renderer
// -------------------------------------------------------------------------
inline void drawBattlefieldLanes(float highlightLane = -1.0f)
{
    glm::mat4 base(1.0f);
    for (int i = 0; i < NUM_LANES; ++i) {
        float cy = LANE_CENTER_Y[i];
        float h = LANE_TOP_Y[i] - LANE_BOTTOM_Y[i];
        glm::vec3 laneCol = (i % 2 == 0) ? glm::vec3(0.32f, 0.65f, 0.24f) : glm::vec3(0.27f, 0.58f, 0.20f);
        float hl = (highlightLane == (float)i) ? 0.25f : 0.0f;

        // Main lane grass strip
        drawPart(g_quadMesh, base, glm::vec2(0.085f, cy), glm::vec2(1.73f, h - 0.01f), 0.0f, laneCol, 0.0f, hl);

        // Dark lane divider strip at bottom edge
        drawPart(g_quadMesh, base, glm::vec2(0.085f, LANE_BOTTOM_Y[i]), glm::vec2(1.73f, 0.008f), 0.0f, glm::vec3(0.18f, 0.38f, 0.12f));

        // Checker tiles to give rich grass texture and sense of grid
        float tileW = 1.73f / 9.0f;
        for (int c = 0; c < 9; ++c) {
            if ((c + i) % 2 == 1) {
                float tx = -0.78f + ((float)c + 0.5f) * tileW;
                drawPart(g_quadMesh, base, glm::vec2(tx, cy), glm::vec2(tileW - 0.006f, h - 0.016f), 0.0f, laneCol * 1.09f, 0.0f, hl);
            }
        }
    }
}

// -------------------------------------------------------------------------
// Dedicated High-Fidelity Menu Renderers
// Sized and positioned with symmetric vertical margins
// -------------------------------------------------------------------------

// Main Menu: Full Castle Environment + Lanes + Home Base + Royal Navy Modal
inline void renderMainMenuUI(float time, glm::vec2 cursorNDC)
{
    // Draw the full castle environment, green battlefield lanes, and home base tower
    drawDetailedEnvironment(time);
    drawBattlefieldLanes();
    drawDetailedHomeBase();

    glm::vec2 modalCenter(0.0f, 0.02f);
    glm::vec2 modalSize(0.72f, 0.68f);
    drawModalFrame(modalCenter, modalSize, glm::vec3(0.14f, 0.18f, 0.26f), "CASTLE SIEGE", "- 2D TOWER DEFENSE -", 1, time);

    drawButton(glm::vec2(0.0f, 0.00f), glm::vec2(0.48f, 0.130f), glm::vec3(0.20f, 0.65f, 0.30f), 1, cursorNDC);
    drawButton(glm::vec2(0.0f, -0.17f), glm::vec2(0.40f, 0.110f), glm::vec3(0.75f, 0.22f, 0.22f), 3, cursorNDC);
}

// Pause Menu: Iron Slate Modal, Portcullis Pillars, "GAME PAUSED" Header
inline void renderPauseMenuUI(float time, glm::vec2 cursorNDC)
{
    glm::vec2 modalCenter(0.0f, 0.00f);
    glm::vec2 modalSize(0.72f, 0.82f);
    drawModalFrame(modalCenter, modalSize, glm::vec3(0.13f, 0.15f, 0.22f), "GAME PAUSED", "- SELECT ACTION -", 4, time);

    drawButton(glm::vec2(0.0f, 0.06f), glm::vec2(0.46f, 0.110f), glm::vec3(0.20f, 0.65f, 0.30f), 1, cursorNDC, "RESUME");
    drawButton(glm::vec2(0.0f, -0.09f), glm::vec2(0.46f, 0.110f), glm::vec3(0.85f, 0.55f, 0.15f), 2, cursorNDC, "RETRY");
    drawButton(glm::vec2(0.0f, -0.24f), glm::vec2(0.46f, 0.110f), glm::vec3(0.75f, 0.22f, 0.22f), 3, cursorNDC, "EXIT");
}

// End-Game Menus:
// Victory: Emerald & Gold Imperial Plaque, Golden Trophy Chalice, "VICTORY"
// Defeat: Gothic Obsidian & Crimson Plaque, Skull & Broken Shield, "DEFEAT"
inline void renderEndMenuUI(bool isWin, float time, glm::vec2 cursorNDC)
{
    glm::vec2 modalCenter(0.0f, 0.02f);
    glm::vec2 modalSize(0.72f, 0.68f);

    if (isWin) {
        drawModalFrame(modalCenter, modalSize, glm::vec3(0.14f, 0.45f, 0.22f), "VICTORY", "- ALL 3 WAVES DEFENDED -", 3, time);
    }
    else {
        drawModalFrame(modalCenter, modalSize, glm::vec3(0.55f, 0.14f, 0.14f), "DEFEAT", "- THE CASTLE HAS FALLEN -", 2, time);
    }

    drawButton(glm::vec2(0.0f, 0.00f), glm::vec2(0.45f, 0.120f), glm::vec3(0.85f, 0.55f, 0.15f), 2, cursorNDC);
    drawButton(glm::vec2(0.0f, -0.17f), glm::vec2(0.45f, 0.110f), glm::vec3(0.25f, 0.30f, 0.40f), 3, cursorNDC);
}

// -------------------------------------------------------------------------
// Top Bar HUD
// -------------------------------------------------------------------------
inline void renderTopBarUI(float time, int coins, bool hasEarnedExtraCoins, int selectedCard, int hoveredCard, int currentWave)
{
    int playerCost[3] = { 10, 20, 30 };
    glm::mat4 base(1.0f);

    // Main top HUD banner background
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.87f), glm::vec2(2.0f, 0.26f), 0.0f, glm::vec3(0.11f, 0.13f, 0.17f));
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.87f), glm::vec2(1.98f, 0.24f), 0.0f, glm::vec3(0.18f, 0.21f, 0.27f), 0.0f, 0.15f);

    // Render 3 Unit Selection Cards
    for (int i = 0; i < 3; ++i) {
        glm::vec2 cardCenter(-0.68f + (float)i * 0.24f, 0.87f);
        glm::vec2 cardSize(0.20f, 0.21f);

        bool unlocked = isCardUnlocked(i, coins, hasEarnedExtraCoins);
        int gray_val = unlocked ? 0 : 1;

        bool isSelected = (selectedCard == i);
        bool isHovered = (hoveredCard == i);

        float scale = 1.0f;
        if (unlocked && (isSelected || isHovered)) {
            scale = 1.05f + 0.02f * std::sin(time * 5.0f);
        }

        float highlight = (unlocked && (isSelected || isHovered)) ? 0.42f : 0.0f;
        glm::vec3 borderColor = isSelected ? glm::vec3(1.0f, 0.85f, 0.25f) : glm::vec3(0.45f, 0.40f, 0.28f);

        // Card outer border
        drawPart(g_quadMesh, base, cardCenter, cardSize * scale, 0.0f, borderColor, 0.0f, highlight, gray_val);
        // Card inner base
        drawPart(g_quadMesh, base, cardCenter, (cardSize - glm::vec2(0.018f)) * scale, 0.0f, glm::vec3(0.26f, 0.28f, 0.34f), 0.0f, highlight, gray_val);

        // Character mini-model inside card
        drawPlayerModel((PlayerType)i, cardCenter + glm::vec2(0.0f, 0.02f), unlocked ? time : 0.0f, 0.0f, false, false, gray_val);

        // Cost coin icon and cost digit
        drawPart(g_circleMesh, base, cardCenter + glm::vec2(-0.055f, -0.068f), glm::vec2(0.016f, 0.016f), 0.0f, glm::vec3(0.98f, 0.80f, 0.15f), 0.0f, 0.0f, gray_val);
        drawNumber(playerCost[i], cardCenter + glm::vec2(-0.015f, -0.068f), 0.85f, glm::vec3(1.0f, 0.95f, 0.8f), gray_val);

        // Procedural padlock icon for locked cards
        if (!hasEarnedExtraCoins && i > 0) {
            glm::vec2 lockPos = cardCenter + glm::vec2(0.055f, 0.060f);
            drawPart(g_circleMesh, base, lockPos + glm::vec2(0.0f, 0.012f), glm::vec2(0.016f, 0.016f), 0.0f, glm::vec3(0.85f, 0.70f, 0.20f));
            drawPart(g_circleMesh, base, lockPos + glm::vec2(0.0f, 0.012f), glm::vec2(0.009f, 0.009f), 0.0f, glm::vec3(0.26f, 0.28f, 0.34f));
            drawPart(g_quadMesh, base, lockPos, glm::vec2(0.024f, 0.020f), 0.0f, glm::vec3(0.85f, 0.70f, 0.20f));
        }
    }

    // Wave Counter Panel (Heraldic War-Standard Badge)
    glm::vec2 waveCenter(0.20f, 0.87f);
    // Soft drop shadow
    drawPart(g_quadMesh, base, waveCenter + glm::vec2(0.004f, -0.005f), glm::vec2(0.26f, 0.15f), 0.0f, glm::vec3(0.05f, 0.06f, 0.08f));
    // Outer metallic silver/steel border
    drawPart(g_quadMesh, base, waveCenter, glm::vec2(0.26f, 0.15f), 0.0f, glm::vec3(0.42f, 0.48f, 0.58f));
    // Inner golden/bronze accent trim
    drawPart(g_quadMesh, base, waveCenter, glm::vec2(0.246f, 0.136f), 0.0f, glm::vec3(0.72f, 0.58f, 0.22f));
    // Deep dark velvet backing
    drawPart(g_quadMesh, base, waveCenter, glm::vec2(0.230f, 0.120f), 0.0f, glm::vec3(0.12f, 0.14f, 0.20f));

    // Heraldic Battle Standard / Flag Icon on the left
    glm::vec2 flagPos = waveCenter + glm::vec2(-0.068f, 0.0f);
    // Wooden pole
    drawPart(g_quadMesh, base, flagPos, glm::vec2(0.005f, 0.088f), 0.0f, glm::vec3(0.62f, 0.44f, 0.22f));
    // Steel spearhead
    drawPart(g_triangleMesh, base, flagPos + glm::vec2(0.0f, 0.046f), glm::vec2(0.012f, 0.016f), 0.0f, glm::vec3(0.88f, 0.90f, 0.98f));
    // Royal cyan pennant flag
    drawPart(g_triangleMesh, base, flagPos + glm::vec2(0.020f, 0.018f), glm::vec2(0.036f, 0.030f), -90.0f, glm::vec3(0.25f, 0.70f, 1.0f));
    // Pennant notch
    drawPart(g_triangleMesh, base, flagPos + glm::vec2(0.032f, 0.018f), glm::vec2(0.016f, 0.020f), -90.0f, glm::vec3(0.12f, 0.14f, 0.20f));

    // Stacked crisp text: "WAVE" at top-right, centered gold number below
    drawText("WAVE", waveCenter + glm::vec2(-0.022f, 0.018f), 0.72f, glm::vec3(0.65f, 0.85f, 1.0f));
    drawNumber(currentWave, waveCenter + glm::vec2(0.018f, -0.024f), 1.40f, glm::vec3(1.0f, 0.92f, 0.35f), 0);

    // Coin Total Panel (Static HUD Coin with zero motion)
    glm::vec2 coinPanel(0.62f, 0.87f);
    drawPart(g_quadMesh, base, coinPanel + glm::vec2(0.004f, -0.005f), glm::vec2(0.27f, 0.15f), 0.0f, glm::vec3(0.05f, 0.06f, 0.08f));
    drawPart(g_quadMesh, base, coinPanel, glm::vec2(0.27f, 0.15f), 0.0f, glm::vec3(0.48f, 0.40f, 0.25f));
    drawPart(g_quadMesh, base, coinPanel, glm::vec2(0.25f, 0.13f), 0.0f, glm::vec3(0.20f, 0.18f, 0.13f));
    drawStaticHUDCoin(glm::vec2(0.535f, 0.87f));
    drawNumber(coins, glm::vec2(0.615f, 0.87f), 1.6f, glm::vec3(1.0f, 0.90f, 0.20f), 0);
}

// -------------------------------------------------------------------------
// Opening Cutscene Sequencing (14.0 Seconds Total Cinematic Timeline)
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
// Opening Cutscene Sequencing (14.0 Seconds Total Cinematic Timeline)
// -------------------------------------------------------------------------
inline void renderCutscene(float time, float cutsceneTimer)
{
    glm::mat4 base(1.0f);
    glm::vec2 cameraOffset(0.0f, 0.0f);

    // Dynamic screen shake during heavy impacts (5.0s - 7.5s)
    if (cutsceneTimer > 5.0f && cutsceneTimer < 7.5f) {
        float shakeIntensity = (7.5f - cutsceneTimer) / 2.5f * 0.005f;
        cameraOffset.x = std::sin(time * 60.0f) * shakeIntensity;
        cameraOffset.y = std::cos(time * 50.0f) * shakeIntensity;
    }

    // 1. Castle environment, checkered battlefield lanes, and home base tower
    drawDetailedEnvironment(time);
    drawBattlefieldLanes();
    drawDetailedHomeBase();

    // 2. Units & Choreography across lanes
    // --- ENEMY WARBAND (Right Side) ---
    float enemyProg = smoothstep(0.0f, 4.2f, cutsceneTimer);
    float warlordX = lerp(0.95f, 0.28f, enemyProg);
    drawEnemyModel(ENEMY_HARD_MELEE, glm::vec2(warlordX, LANE_CENTER_Y[1]) + cameraOffset, time * 1.4f, 0.0f);

    float gargoyleHover = std::sin(time * 6.0f) * 0.025f;
    float gargoyleX = warlordX + 0.18f;
    if (gargoyleX > LANE_RIGHT_X) gargoyleX = LANE_RIGHT_X;
    drawEnemyModel(ENEMY_EASY_UPPER, glm::vec2(gargoyleX, LANE_CENTER_Y[0] + 0.04f + gargoyleHover) + cameraOffset, time * 1.5f, 0.0f);

    float goblinX = warlordX + 0.16f;
    if (goblinX > LANE_RIGHT_X) goblinX = LANE_RIGHT_X;
    drawEnemyModel(ENEMY_MEDIUM_LOWER, glm::vec2(goblinX, LANE_CENTER_Y[2]) + cameraOffset, time * 1.3f, 0.0f);

    // --- PLAYER DEFENDERS (Left Side) ---
    float defenderProg = smoothstep(4.5f, 6.2f, cutsceneTimer);
    float defenderX = lerp(-0.62f, -0.32f, defenderProg);
    drawPlayerModel(PLAYER_DEFENSE, glm::vec2(defenderX, LANE_CENTER_Y[2]) + cameraOffset, time, 0.0f, false, false);

    float knightProg = smoothstep(4.2f, 6.0f, cutsceneTimer);
    float knightX = lerp(-0.72f, -0.15f, knightProg);
    bool swordDrawn = (cutsceneTimer > 5.5f);
    drawPlayerModel(PLAYER_MELEE, glm::vec2(knightX, LANE_CENTER_Y[1]) + cameraOffset, time * 1.6f, 0.0f, swordDrawn, false);

    // Ranger Lyra stationed on Lane 0
    if (cutsceneTimer >= 8.5f) {
        float archerProg = smoothstep(8.5f, 9.8f, cutsceneTimer);
        float archerX = lerp(-0.72f, -0.42f, archerProg);
        drawPlayerModel(PLAYER_RANGED, glm::vec2(archerX, LANE_CENTER_Y[0]) + cameraOffset, time * 1.2f, 0.0f, false, false);
    }

    // 3. Combat Action & Projectile Clashes
    // Act 2: Warlord's fireball fired at 4.8s across Lane 1
    if (cutsceneTimer > 4.8f && cutsceneTimer < 8.0f) {
        if (cutsceneTimer < 5.15f) {
            float flash = (5.15f - cutsceneTimer) / 0.35f * 0.40f;
            drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, flash);
        }

        Projectile warningShot;
        warningShot.lane = 1;
        warningShot.x = 0.28f - (cutsceneTimer - 4.8f) * 0.55f;
        warningShot.fromPlayer = false;
        warningShot.isHigh = false;
        warningShot.damage = 1;
        warningShot.hitAnimTimer = 0.0f;
        bool intercepted = (warningShot.x <= knightX + 0.06f);
        warningShot.hitting = intercepted;
        warningShot.alive = true;

        if (intercepted) {
            warningShot.x = knightX + 0.06f;
            warningShot.hitAnimTimer = std::max(0.0f, 0.20f - (cutsceneTimer - 5.6f) * 0.3f);
        }
        drawProjectile(warningShot, time);
    }

    // Act 3: Archer volley fired by Ranger Lyra in Lane 0 (9.8s and 11.0s)
    if (cutsceneTimer > 9.8f && cutsceneTimer < 13.5f) {
        float archerX = -0.42f;

        if (cutsceneTimer < 11.2f) {
            Projectile arrow1;
            arrow1.lane = 0;
            arrow1.x = archerX + (cutsceneTimer - 9.8f) * 0.85f;
            arrow1.fromPlayer = true;
            arrow1.isHigh = false;
            arrow1.damage = 1;
            arrow1.hitAnimTimer = 0.0f;
            arrow1.hitting = (arrow1.x >= gargoyleX - 0.04f);
            arrow1.alive = true;
            if (arrow1.hitting) {
                arrow1.x = gargoyleX - 0.04f;
                arrow1.hitAnimTimer = std::max(0.0f, 0.20f - (cutsceneTimer - 10.5f) * 0.3f);
            }
            drawProjectile(arrow1, time);
        }

        if (cutsceneTimer > 11.0f) {
            Projectile arrow2;
            arrow2.lane = 0;
            arrow2.x = archerX + (cutsceneTimer - 11.0f) * 0.95f;
            arrow2.fromPlayer = true;
            arrow2.isHigh = false;
            arrow2.damage = 1;
            arrow2.hitAnimTimer = 0.0f;
            arrow2.hitting = false;
            arrow2.alive = (arrow2.x < LANE_RIGHT_X);
            drawProjectile(arrow2, time);
        }
    }

    // 4. Cinematic Dialogue Box (Bottom NDC)
    glm::vec2 boxCenter = glm::vec2(0.0f, -0.77f) + cameraOffset;
    glm::vec2 boxSize(1.72f, 0.23f);

    drawPart(g_quadMesh, base, boxCenter + glm::vec2(0.006f, -0.008f), boxSize + glm::vec2(0.016f, 0.016f), 0.0f, glm::vec3(0.03f, 0.04f, 0.06f));
    drawPart(g_quadMesh, base, boxCenter, boxSize + glm::vec2(0.012f, 0.012f), 0.0f, glm::vec3(0.72f, 0.58f, 0.22f));
    drawPart(g_quadMesh, base, boxCenter, boxSize, 0.0f, glm::vec3(0.10f, 0.12f, 0.16f));

    glm::vec2 portraitCenter = glm::vec2(-0.68f, -0.77f) + cameraOffset;
    drawPart(g_quadMesh, base, portraitCenter, glm::vec2(0.18f, 0.18f), 0.0f, glm::vec3(0.20f, 0.24f, 0.32f));
    drawPart(g_quadMesh, base, portraitCenter, glm::vec2(0.16f, 0.16f), 0.0f, glm::vec3(0.08f, 0.10f, 0.14f));

    if (cutsceneTimer < 4.8f) {
        drawEnemyModel(ENEMY_HARD_MELEE, portraitCenter, time * 1.2f, 0.0f);
        drawText("[WARLORD MALIK]", glm::vec2(-0.55f, -0.71f) + cameraOffset, 0.90f, glm::vec3(1.0f, 0.35f, 0.35f));
        drawText("SURRENDER THE CRYSTAL, MORTALS!", glm::vec2(-0.55f, -0.77f) + cameraOffset, 0.85f, glm::vec3(1.0f, 0.88f, 0.88f));
        drawText("YOUR CRUMBLING WALLS CANNOT SAVE YOU!", glm::vec2(-0.55f, -0.82f) + cameraOffset, 0.78f, glm::vec3(0.85f, 0.70f, 0.70f));
    }
    else if (cutsceneTimer < 9.2f) {
        drawPlayerModel(PLAYER_MELEE, portraitCenter, time * 1.2f, 0.0f, true, false);
        drawText("[COMMANDER VALEN]", glm::vec2(-0.55f, -0.71f) + cameraOffset, 0.90f, glm::vec3(0.40f, 0.85f, 1.0f));
        drawText("NEVER! THE SIEGE LINE WILL NEVER BREAK!", glm::vec2(-0.55f, -0.77f) + cameraOffset, 0.85f, glm::vec3(0.88f, 0.95f, 1.0f));
        drawText("KNIGHTS, RAISE YOUR SHIELDS AND DRAW YOUR STEEL!", glm::vec2(-0.55f, -0.82f) + cameraOffset, 0.78f, glm::vec3(0.70f, 0.85f, 0.95f));
    }
    else {
        drawPlayerModel(PLAYER_RANGED, portraitCenter, time * 1.2f, 0.0f, false, false);
        drawText("[RANGER LYRA]", glm::vec2(-0.55f, -0.71f) + cameraOffset, 0.90f, glm::vec3(1.0f, 0.88f, 0.25f));
        drawText("BOWSTRINGS DRAWN! ARROWS AT THE READY!", glm::vec2(-0.55f, -0.77f) + cameraOffset, 0.85f, glm::vec3(1.0f, 0.96f, 0.85f));
        drawText("FOR THE KINGDOM! DEFEND THE GATES!", glm::vec2(-0.55f, -0.82f) + cameraOffset, 0.78f, glm::vec3(0.95f, 0.85f, 0.65f));
    }

    if (cutsceneTimer >= 12.0f) {
        float pulse = 1.0f + 0.08f * std::sin(time * 10.0f);
        drawCenteredTextWithShadow("PREPARE FOR BATTLE! WAVE 1 INCOMING...", glm::vec2(0.0f, 0.45f) + cameraOffset, 0.85f * pulse, glm::vec3(1.0f, 0.90f, 0.25f));
    }

    drawText("[ESC] SKIP", glm::vec2(0.72f, 0.88f), 0.75f, glm::vec3(0.55f, 0.60f, 0.70f));

    // Cinematic letterbox bars
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.96f), glm::vec2(2.0f, 0.08f), 0.0f, glm::vec3(0.04f, 0.04f, 0.06f));
    drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.96f), glm::vec2(2.0f, 0.08f), 0.0f, glm::vec3(0.04f, 0.04f, 0.06f));
}