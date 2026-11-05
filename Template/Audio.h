#pragma once
#include <string>
#include <cstdio>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

// -------------------------------------------------------------------------
// Global Sound & Volume State (Bonus Requirement: Page 14)
// -------------------------------------------------------------------------
extern int  g_audioVolume; // 0 to 100%
extern bool g_audioMuted;  // true if muted

// Helper to resolve audio file path across working directory variations
inline std::string getAudioPath(const char* filename)
{
    std::string p1 = std::string("audio/") + filename;
    FILE* f = fopen(p1.c_str(), "rb");
    if (f) { fclose(f); return p1; }

    std::string p2 = std::string("../audio/") + filename;
    f = fopen(p2.c_str(), "rb");
    if (f) { fclose(f); return p2; }

    std::string p3 = std::string("../Template/audio/") + filename;
    f = fopen(p3.c_str(), "rb");
    if (f) { fclose(f); return p3; }

    return p1;
}

// -------------------------------------------------------------------------
// Volume Control & Synchronization
// -------------------------------------------------------------------------
inline void updateAudioVolume()
{
#ifdef _WIN32
    int mciVol = g_audioMuted ? 0 : (g_audioVolume * 10);
    if (mciVol < 0) mciVol = 0;
    if (mciVol > 1000) mciVol = 1000;

    const char* aliases[] = {
        "bgm", "sfx_select", "sfx_deploy", "sfx_coin",
        "sfx_hit", "sfx_arrow", "sfx_death", "sfx_win", "sfx_lose"
    };

    char cmd[128];
    for (int i = 0; i < 9; ++i) {
        snprintf(cmd, sizeof(cmd), "setaudio %s volume to %d", aliases[i], mciVol);
        mciSendStringA(cmd, NULL, 0, NULL);
    }

    if (g_audioMuted) {
        mciSendStringA("pause bgm", NULL, 0, NULL);
    } else if (g_audioVolume > 0) {
        mciSendStringA("play bgm repeat", NULL, 0, NULL);
    }
#endif
}

inline void setAudioVolume(int vol)
{
    g_audioVolume = std::max(0, std::min(100, vol));
    if (g_audioVolume > 0) {
        g_audioMuted = false;
    }
    updateAudioVolume();
}

inline void toggleAudioMute()
{
    g_audioMuted = !g_audioMuted;
    updateAudioVolume();
}

// -------------------------------------------------------------------------
// Initialization & Playback
// -------------------------------------------------------------------------
inline void initAudio()
{
#ifdef _WIN32
    // Close any previous instances
    mciSendStringA("close all", NULL, 0, NULL);

    // Open background music and sound effects with distinct aliases for concurrent playback
    struct AudioMap { const char* alias; const char* file; };
    AudioMap map[] = {
        { "bgm",        "bgm.wav" },
        { "sfx_select", "select.wav" },
        { "sfx_deploy", "deploy.wav" },
        { "sfx_coin",   "coin.wav" },
        { "sfx_hit",    "hit.wav" },
        { "sfx_arrow",  "arrow.wav" },
        { "sfx_death",  "death.wav" },
        { "sfx_win",    "win.wav" },
        { "sfx_lose",   "lose.wav" }
    };

    char cmd[256];
    for (int i = 0; i < 9; ++i) {
        std::string fullPath = getAudioPath(map[i].file);
        snprintf(cmd, sizeof(cmd), "open \"%s\" type waveaudio alias %s", fullPath.c_str(), map[i].alias);
        mciSendStringA(cmd, NULL, 0, NULL);
    }

    updateAudioVolume();
#endif
}

inline void playBGM()
{
#ifdef _WIN32
    if (!g_audioMuted && g_audioVolume > 0) {
        mciSendStringA("play bgm repeat", NULL, 0, NULL);
    }
#endif
}

inline void stopBGM()
{
#ifdef _WIN32
    mciSendStringA("stop bgm", NULL, 0, NULL);
#endif
}

inline void playAudioAlias(const char* alias)
{
    (void)alias;
#ifdef _WIN32
    if (g_audioMuted || g_audioVolume <= 0) return;
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "play %s from 0", alias);
    mciSendStringA(cmd, NULL, 0, NULL);
#endif
}

// Sound interaction triggers
inline void playAudioSelect() { playAudioAlias("sfx_select"); }
inline void playAudioDeploy() { playAudioAlias("sfx_deploy"); }
inline void playAudioCoin()   { playAudioAlias("sfx_coin"); }
inline void playAudioHit()    { playAudioAlias("sfx_hit"); }
inline void playAudioArrow()  { playAudioAlias("sfx_arrow"); }
inline void playAudioDeath()  { playAudioAlias("sfx_death"); }
inline void playAudioWin()    { playAudioAlias("sfx_win"); }
inline void playAudioLose()   { playAudioAlias("sfx_lose"); }
