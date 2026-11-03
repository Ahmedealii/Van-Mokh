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
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.4f, yt), glm::vec2(w * 0.8f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.4f, ym), glm::vec2(w * 0.8f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.4f, yb), glm::vec2(w * 0.8f, t), 0.0f, col);
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
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.4f, yt), glm::vec2(w * 0.8f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xl + w * 0.4f, yb), glm::vec2(w * 0.8f, t), 0.0f, col);
        drawPart(g_quadMesh, base, glm::vec2(xr, ym), glm::vec2(t, h * 0.8f), 0.0f, col);
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
        drawPart(g_quadMesh, base, glm::vec2(xm + w * 0.20f, yb + h * 0.22f), glm::vec2(t, h * 0.55f), 35.0f, col);
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
        drawDigit(c - '0', glm::vec2(xm, ym), scale * 1.2f, col);
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

// -------------------------------------------------------------------------
// Collectible Gold Coin Animation
// Pulses scale at sin(time * 6.0f) dropped by defeated enemies
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
            // Enemy High Magic Orb (Pulsing violet/arcane sphere with rotating outer spark)
            float orbPulse = 1.0f + 0.12f * std::sin(time * 12.0f);
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.024f * orbPulse, 0.024f * orbPulse), 0.0f, glm::vec3(0.70f, 0.15f, 0.85f));
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.016f, 0.016f), 0.0f, glm::vec3(0.98f, 0.40f, 0.95f));
            drawPart(g_triangleMesh, base, glm::vec2(p.x + 0.015f, y), glm::vec2(0.018f, 0.018f), time * 320.0f, glm::vec3(1.0f, 0.85f, 1.0f));
        }
        else {
            // Enemy Low Magic Orb (Fiery orange/red fireball with blazing trail)
            float firePulse = 1.0f + 0.10f * std::sin(time * 10.0f);
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.024f * firePulse, 0.024f * firePulse), 0.0f, glm::vec3(1.0f, 0.35f, 0.05f));
            drawPart(g_circleMesh, base, glm::vec2(p.x, y), glm::vec2(0.015f, 0.015f), 0.0f, glm::vec3(1.0f, 0.88f, 0.20f));
            drawPart(g_triangleMesh, base, glm::vec2(p.x + 0.018f, y), glm::vec2(0.022f, 0.020f), -90.0f, glm::vec3(1.0f, 0.92f, 0.30f));
        }
    }
}

// -------------------------------------------------------------------------
// Custom Geometric Button System & Cursor Hit Testing
// labelCode 1: Play/Resume Triangle
// labelCode 2: Retry/Pause Ring (Circular arrow loop)
// labelCode 3: Exit Cross (Angled crossing quads)
// -------------------------------------------------------------------------
inline void drawButton(glm::vec2 center, glm::vec2 size, glm::vec3 color, int labelCode, glm::vec2 cursorNDC)
{
    glm::mat4 base(1.0f);
    bool hovered = isPointInBox(cursorNDC, center, size);
    float highlight = hovered ? 0.38f : 0.0f;
    glm::vec2 currentSize = hovered ? size * 1.04f : size;

    // Glowing border feedback
    glm::vec3 borderCol = hovered ? glm::vec3(1.0f, 0.90f, 0.40f) : glm::vec3(0.40f, 0.44f, 0.52f);
    drawPart(g_quadMesh, base, center, currentSize + glm::vec2(0.016f, 0.016f), 0.0f, borderCol, 0.0f, highlight);
    // Button interior face
    drawPart(g_quadMesh, base, center, currentSize, 0.0f, color, 0.0f, hovered ? 0.20f : 0.0f);

    glm::vec3 iconCol(1.0f, 1.0f, 1.0f);

    if (labelCode == 1) {
        // Play / Resume: Right-pointing Play Triangle + Text
        drawPart(g_triangleMesh, base, center + glm::vec2(-0.085f, 0.0f), glm::vec2(0.045f, 0.045f), -90.0f, iconCol);
        drawText("PLAY", center + glm::vec2(-0.025f, -0.016f), 1.05f, iconCol);
    }
    else if (labelCode == 2) {
        // Retry / Restart: Retry/Pause Ring with arrow tip + Text
        drawPart(g_circleMesh, base, center + glm::vec2(-0.085f, 0.0f), glm::vec2(0.038f, 0.038f), 0.0f, iconCol);
        drawPart(g_circleMesh, base, center + glm::vec2(-0.085f, 0.0f), glm::vec2(0.025f, 0.025f), 0.0f, color);
        // Arrow tip at top-right of ring
        drawPart(g_triangleMesh, base, center + glm::vec2(-0.065f, 0.022f), glm::vec2(0.024f, 0.024f), 30.0f, iconCol);
        drawText("RETRY", center + glm::vec2(-0.035f, -0.016f), 1.05f, iconCol);
    }
    else if (labelCode == 3) {
        // Exit: Crossed Quads (Exit Cross) + Text
        drawPart(g_quadMesh, base, center + glm::vec2(-0.085f, 0.0f), glm::vec2(0.040f, 0.009f), 45.0f, iconCol);
        drawPart(g_quadMesh, base, center + glm::vec2(-0.085f, 0.0f), glm::vec2(0.040f, 0.009f), -45.0f, iconCol);
        drawText("EXIT", center + glm::vec2(-0.025f, -0.016f), 1.05f, iconCol);
    }
}

// -------------------------------------------------------------------------
// Top Bar HUD
// 3 Unit Cards (Melee: 10, Ranged: 20, Defense: 30)
// Locking & Grayscale, Selection Highlight, Wave & Coin counters
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
        glm::vec2 cardCenter(-0.68f + i * 0.24f, 0.87f);
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

        // Procedural padlock icon for locked cards (Ranged & Defense before extra coins collected)
        if (!hasEarnedExtraCoins && i > 0) {
            glm::vec2 lockPos = cardCenter + glm::vec2(0.055f, 0.060f);
            drawPart(g_circleMesh, base, lockPos + glm::vec2(0.0f, 0.012f), glm::vec2(0.016f, 0.016f), 0.0f, glm::vec3(0.85f, 0.70f, 0.20f));
            drawPart(g_circleMesh, base, lockPos + glm::vec2(0.0f, 0.012f), glm::vec2(0.009f, 0.009f), 0.0f, glm::vec3(0.26f, 0.28f, 0.34f));
            drawPart(g_quadMesh, base, lockPos, glm::vec2(0.024f, 0.020f), 0.0f, glm::vec3(0.85f, 0.70f, 0.20f));
        }
    }

    // Wave Counter Panel
    glm::vec2 waveCenter(0.20f, 0.87f);
    drawPart(g_quadMesh, base, waveCenter, glm::vec2(0.24f, 0.14f), 0.0f, glm::vec3(0.24f, 0.28f, 0.36f));
    drawPart(g_quadMesh, base, waveCenter, glm::vec2(0.22f, 0.12f), 0.0f, glm::vec3(0.15f, 0.18f, 0.24f));
    drawText("WAVE", waveCenter + glm::vec2(-0.088f, 0.015f), 0.80f, glm::vec3(0.40f, 0.85f, 1.0f));
    drawNumber(currentWave, waveCenter + glm::vec2(-0.020f, -0.025f), 1.25f, glm::vec3(0.40f, 0.85f, 1.0f));

    // Coin Total Panel
    glm::vec2 coinPanel(0.62f, 0.87f);
    drawPart(g_quadMesh, base, coinPanel, glm::vec2(0.26f, 0.14f), 0.0f, glm::vec3(0.28f, 0.25f, 0.18f));
    drawPart(g_quadMesh, base, coinPanel, glm::vec2(0.24f, 0.12f), 0.0f, glm::vec3(0.18f, 0.16f, 0.12f));
    drawCollectibleCoin(glm::vec2(0.54f, 0.87f), time);
    drawNumber(coins, glm::vec2(0.61f, 0.87f), 1.6f, glm::vec3(1.0f, 0.90f, 0.20f), 0);
}

// -------------------------------------------------------------------------
// Opening Cutscene Sequencing (14.0 Seconds Total Timeline)
// 0.0s - 4.5s : Enemy Vanguard arrives, Warlord demands surrender.
// 4.5s - 9.5s : Screen flash, warning shot fired, Commander steps forward.
// 9.5s - 14.0s: Archer takes defensive perch, battle horns sound.
// -------------------------------------------------------------------------
inline void renderCutscene(float time, float cutsceneTimer)
{
    drawEnvironment(time);
    drawHomeBase();

    glm::mat4 base(1.0f);
    glm::vec2 cameraOffset(0.0f, 0.0f);

    // Screen shake during the dramatic warning blast (5.0s - 8.5s)
    if (cutsceneTimer > 5.0f && cutsceneTimer < 8.5f) {
        cameraOffset.x = std::sin(time * 50.0f) * 0.003f;
        cameraOffset.y = std::cos(time * 45.0f) * 0.003f;
    }

    // Gentle fade-in for battleground
    float bgFade = smoothstep(0.0f, 1.8f, cutsceneTimer);
    drawPart(g_quadMesh, base, glm::vec2(0.08f, -0.15f) + cameraOffset, glm::vec2(1.85f, 1.40f), 0.0f, glm::vec3(0.28f, 0.58f, 0.22f) * bgFade);

    // Castle spire crystal bobbing with mystical glow
    float crystalBob = std::sin(time * 4.0f) * 0.02f;
    drawPart(g_triangleMesh, base, glm::vec2(-0.72f, 0.05f + crystalBob) + cameraOffset, glm::vec2(0.08f, 0.12f), 0.0f, glm::vec3(0.20f, 0.85f, 1.0f));
    drawPart(g_triangleMesh, base, glm::vec2(-0.72f, -0.07f + crystalBob) + cameraOffset, glm::vec2(0.08f, 0.12f), 180.0f, glm::vec3(0.15f, 0.65f, 0.95f));

    // Unit approach animations
    float enemyProg = smoothstep(0.0f, 5.5f, cutsceneTimer);
    float enemyX = lerp(0.85f, 0.32f, enemyProg);

    float knightProg = smoothstep(0.0f, 6.0f, cutsceneTimer);
    float knightX = lerp(-0.52f, -0.22f, knightProg);

    // Defensive gate guardian
    drawPlayerModel(PLAYER_DEFENSE, glm::vec2(-0.62f, -0.28f) + cameraOffset, time, 0.0f, false, false);

    // Advancing player knight
    bool swordDrawn = (cutsceneTimer > 6.0f && cutsceneTimer < 8.5f);
    drawPlayerModel(PLAYER_MELEE, glm::vec2(knightX, 0.0f) + cameraOffset, time * 1.5f, 0.0f, swordDrawn, false);

    // Approaching enemy forces
    drawEnemyModel(ENEMY_HARD_MELEE, glm::vec2(enemyX, 0.0f) + cameraOffset, time * 1.5f, 0.0f);
    drawEnemyModel(ENEMY_EASY_UPPER, glm::vec2(enemyX + 0.16f, 0.22f) + cameraOffset, time * 1.5f, 0.0f);

    // Archer joins position at castle parapet after 9.0s
    if (cutsceneTimer >= 9.0f) {
        float archerProg = smoothstep(9.0f, 10.5f, cutsceneTimer);
        float archerY = lerp(0.40f, 0.22f, archerProg);
        drawPlayerModel(PLAYER_RANGED, glm::vec2(-0.64f, archerY) + cameraOffset, time * 1.2f, 0.0f, false, false);
    }

    // Warning shot & flash impact
    if (cutsceneTimer > 5.0f && cutsceneTimer < 8.5f) {
        if (cutsceneTimer < 5.25f) {
            float flashAlpha = 0.35f * (5.25f - cutsceneTimer) / 0.25f;
            drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.0f), glm::vec2(2.0f, 2.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, flashAlpha);
        }
        Projectile warningShot;
        warningShot.lane = 1;
        warningShot.x = enemyX - (cutsceneTimer - 5.0f) * 0.28f;
        warningShot.fromPlayer = false;
        warningShot.isHigh = false;
        warningShot.damage = 1;
        warningShot.hitAnimTimer = 0.0f;
        warningShot.hitting = (warningShot.x <= knightX + 0.08f);
        warningShot.alive = true;
        drawProjectile(warningShot, time);
    }

    // Cinematic dialogue box at bottom NDC space
    glm::vec2 boxCenter = glm::vec2(0.0f, -0.76f) + cameraOffset;
    glm::vec2 boxSize(1.72f, 0.24f);
    drawPart(g_quadMesh, base, boxCenter, boxSize + glm::vec2(0.015f, 0.015f), 0.0f, glm::vec3(0.35f, 0.38f, 0.46f));
    drawPart(g_quadMesh, base, boxCenter, boxSize, 0.0f, glm::vec3(0.10f, 0.12f, 0.16f));

    // Portrait frame on the left
    glm::vec2 portraitCenter = glm::vec2(-0.68f, -0.76f) + cameraOffset;
    drawPart(g_quadMesh, base, portraitCenter, glm::vec2(0.18f, 0.18f), 0.0f, glm::vec3(0.22f, 0.25f, 0.32f));

    // Dialogue sequencing across 3 distinct story phases
    if (cutsceneTimer < 4.8f) {
        // Phase 1: Enemy Warlord speaks
        drawEnemyModel(ENEMY_HARD_MELEE, portraitCenter, time, 0.0f);
        drawText("[WARLORD MALIK]", glm::vec2(-0.55f, -0.70f) + cameraOffset, 0.90f, glm::vec3(0.95f, 0.35f, 0.35f));
        drawText("SURRENDER THE CASTLE MORTALS!", glm::vec2(-0.55f, -0.76f) + cameraOffset, 0.85f, glm::vec3(1.0f, 0.85f, 0.85f));
        drawText("YOUR CRUMBLING WALLS CANNOT SAVE YOU!", glm::vec2(-0.55f, -0.81f) + cameraOffset, 0.80f, glm::vec3(0.85f, 0.70f, 0.70f));
    }
    else if (cutsceneTimer < 9.5f) {
        // Phase 2: Commander Knight responds
        drawPlayerModel(PLAYER_MELEE, portraitCenter, time, 0.0f, false, false);
        drawText("[COMMANDER VALEN]", glm::vec2(-0.55f, -0.70f) + cameraOffset, 0.90f, glm::vec3(0.40f, 0.80f, 1.0f));
        drawText("NEVER! KNIGHTS, DRAW YOUR BLADES!", glm::vec2(-0.55f, -0.76f) + cameraOffset, 0.85f, glm::vec3(0.85f, 0.95f, 1.0f));
        drawText("DEFEND THE GATES TO THE LAST MAN!", glm::vec2(-0.55f, -0.81f) + cameraOffset, 0.80f, glm::vec3(0.70f, 0.85f, 0.95f));
    }
    else {
        // Phase 3: Archer reinforces siege lines
        drawPlayerModel(PLAYER_RANGED, portraitCenter, time, 0.0f, false, false);
        drawText("[RANGER LYRA]", glm::vec2(-0.55f, -0.70f) + cameraOffset, 0.90f, glm::vec3(1.0f, 0.85f, 0.25f));
        drawText("BOWS DRAWN! ARROWS AT THE READY!", glm::vec2(-0.55f, -0.76f) + cameraOffset, 0.85f, glm::vec3(1.0f, 0.95f, 0.80f));
        drawText("BATTLE COMMENCES! HOLD THE LINE!", glm::vec2(-0.55f, -0.81f) + cameraOffset, 0.80f, glm::vec3(0.95f, 0.85f, 0.65f));
    }

    // Cinematic top/bottom letterbox bars
    drawPart(g_quadMesh, base, glm::vec2(0.0f, 0.96f), glm::vec2(2.0f, 0.08f), 0.0f, glm::vec3(0.05f, 0.05f, 0.07f));
    drawPart(g_quadMesh, base, glm::vec2(0.0f, -0.96f), glm::vec2(2.0f, 0.08f), 0.0f, glm::vec3(0.05f, 0.05f, 0.07f));
}