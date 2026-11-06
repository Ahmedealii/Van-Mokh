#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <vector>
#include <iostream>

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

// Multi-path file finder that checks both MP3 and WAV variants and relative directories
inline std::string findSoundFile(const std::vector<std::string>& candidates)
{
#ifdef _WIN32
    const char* prefixPaths[] = {
        "",
        "assets\\audio\\",
        "audio\\",
        "..\\..\\assets\\audio\\",
        "..\\..\\audio\\",
        "Template\\assets\\audio\\",
        "Template\\audio\\",
        "..\\Template\\assets\\audio\\",
        "..\\Template\\audio\\",
        "..\\assets\\audio\\",
        "..\\audio\\"
    };

    for (const char* pre : prefixPaths) {
        for (size_t i = 0; i < candidates.size(); ++i) {
            std::string full = std::string(pre) + candidates[i];
            DWORD attrs = GetFileAttributesA(full.c_str());
            if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                char absPath[MAX_PATH] = {};
                if (GetFullPathNameA(full.c_str(), MAX_PATH, absPath, nullptr)) {
                    return std::string(absPath);
                }
                return full;
            }
        }
    }
#else
    (void)candidates;
#endif
    return "";
}

inline void playMciSound(const char* baseAlias, const std::vector<std::string>& candidates)
{
#ifdef _WIN32
    if (g_audioMuted || g_audioVolume <= 0) return;

    std::string path = findSoundFile(candidates);
    if (path.empty()) {
        std::cerr << "[Audio] Sound file not found for: " << baseAlias << '\n';
        return;
    }

    // Rotating alias pool (0 to 3) so simultaneous rapid attacks, hits, or deaths do not cut each other off
    static int s_channelCounter = 0;
    std::string alias = std::string(baseAlias) + "_" + std::to_string((s_channelCounter++) % 4);

    // Close any previous instance on this channel
    std::string closeCmd = std::string("close ") + alias;
    mciSendStringA(closeCmd.c_str(), nullptr, 0, nullptr);

    // Open file using modern DirectShow MPEG audio engine
    std::string openCmd = std::string("open \"") + path + "\" type mpegvideo alias " + alias;
    MCIERROR err = mciSendStringA(openCmd.c_str(), nullptr, 0, nullptr);
    if (err) {
        openCmd = std::string("open \"") + path + "\" alias " + alias;
        err = mciSendStringA(openCmd.c_str(), nullptr, 0, nullptr);
    }
    if (err) {
        char errBuf[256] = {};
        mciGetErrorStringA(err, errBuf, sizeof(errBuf));
        std::cerr << "[Audio] Open error on " << alias << " (" << path << "): " << errBuf << '\n';
        return;
    }

    int mciVol = g_audioMuted ? 0 : (g_audioVolume * 10);
    std::string volCmd = std::string("setaudio ") + alias + " volume to " + std::to_string(mciVol);
    mciSendStringA(volCmd.c_str(), nullptr, 0, nullptr);

    std::string playCmd = std::string("play ") + alias + " from 0";
    mciSendStringA(playCmd.c_str(), nullptr, 0, nullptr);
#else
    (void)baseAlias; (void)candidates;
#endif
}

inline void updateAudioVolume()
{
#ifdef _WIN32
    int mciVol = g_audioMuted ? 0 : (g_audioVolume * 10);
    if (mciVol < 0) mciVol = 0;
    if (mciVol > 1000) mciVol = 1000;

    const char* aliases[] = {
        "vanMokhBgm", "sfx_coin", "sfx_death", "sfx_win", "sfx_lose",
        "sfx_deploy", "sfx_select", "sfx_hit", "sfx_arrow"
    };

    char cmd[128];
    for (int i = 0; i < 9; ++i) {
        snprintf(cmd, sizeof(cmd), "setaudio %s volume to %d", aliases[i], mciVol);
        mciSendStringA(cmd, nullptr, 0, nullptr);
    }

    // Also update hardware wave volume
    DWORD dVol = g_audioMuted ? 0 : (DWORD)((g_audioVolume * 65535) / 100);
    waveOutSetVolume(0, (dVol & 0xFFFF) | ((dVol & 0xFFFF) << 16));
#endif
}

inline void setAudioVolume(int vol)
{
    if (vol < 0) vol = 0;
    if (vol > 100) vol = 100;
    g_audioVolume = vol;
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

inline void initAudio()
{
    updateAudioVolume();
    std::cout << "[Audio] Sound system initialized with MCI multi-track audio." << std::endl;
}

// -------------------------------------------------------------------------
// Sound Triggers (Mapping your 4 uploaded Clash Royale sound files + extras)
// -------------------------------------------------------------------------

// 1. Coin collect (variant) Sound Effect
inline void playAudioCoin()
{
    std::vector<std::string> files = {
        "coin.mp3",
        "Coin collect (variant) Sound Effect.mp3",
        "coin.wav",
        "Coin collect (variant) Sound Effect.wav"
    };
    playMciSound("sfx_coin", files);
}

// 2. Death Scream Sound Effect
inline void playAudioDeath()
{
    std::vector<std::string> files = {
        "death.mp3",
        "Death Scream Sound Effect.mp3",
        "death.wav",
        "Death Scream Sound Effect.wav"
    };
    playMciSound("sfx_death", files);
}

// 3. Clash Royale Soundtrack - Win
inline void playAudioWin()
{
    std::vector<std::string> files = {
        "win.mp3",
        "Clash Royale Soundtrack - Win.mp3",
        "win.wav",
        "Clash Royale Soundtrack - Win.wav"
    };
    playMciSound("sfx_win", files);
}

// 4. Clash Royale Lose - Meme Sound Effect
inline void playAudioLose()
{
    std::vector<std::string> files = {
        "lose.mp3",
        "Clash Royale Lose - Meme Sound Effect.mp3",
        "lose.wav",
        "Clash Royale Lose - Meme Sound Effect.wav"
    };
    playMciSound("sfx_lose", files);
}

// Additional interaction sounds
inline void playAudioDeploy()
{
    std::vector<std::string> files = { "deploy.wav", "deploy.mp3" };
    playMciSound("sfx_deploy", files);
}

inline void playAudioSelect()
{
    std::vector<std::string> files = { "select.wav", "select.mp3" };
    playMciSound("sfx_select", files);
}

inline void playAudioHit()
{
    std::vector<std::string> files = { "hit.wav", "hit.mp3" };
    playMciSound("sfx_hit", files);
}

inline void playAudioArrow()
{
    std::vector<std::string> files = { "arrow.wav", "arrow.mp3" };
    playMciSound("sfx_arrow", files);
}

inline void playBGM() {}
inline void stopBGM() {}