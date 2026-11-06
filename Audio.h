#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#include <direct.h>
#pragma comment(lib, "winmm.lib")
#endif

// -------------------------------------------------------------------------
// Global Sound & Volume State (Bonus Requirement: Page 14)
// -------------------------------------------------------------------------
extern int  g_audioVolume; // 0 to 100%
extern bool g_audioMuted;  // true if muted

// Helper to check if a file exists
inline bool audioFileExists(const std::string& path)
{
    std::ifstream f(path.c_str(), std::ios::binary);
    return f.good();
}

// Helper to resolve audio file path across working directory variations
inline std::string getAudioPath(const char* filename)
{
    std::string p1 = std::string("audio/") + filename;
    if (audioFileExists(p1)) return p1;

    std::string p2 = std::string("../audio/") + filename;
    if (audioFileExists(p2)) return p2;

    std::string p3 = std::string("../Template/audio/") + filename;
    if (audioFileExists(p3)) return p3;

    return p1;
}

// -------------------------------------------------------------------------
// Built-in WAV Synthesizer (Auto-generates sounds if files are missing!)
// -------------------------------------------------------------------------
inline void writePcmWav(const std::string& path, const std::vector<short>& samples, int sampleRate = 44100)
{
    FILE* f = fopen(path.c_str(), "wb");
    if (!f) return;
    int dataSize = (int)samples.size() * 2;
    int chunkSize = 36 + dataSize;
    short audioFormat = 1;
    short numChannels = 1;
    int byteRate = sampleRate * 2;
    short blockAlign = 2;
    short bitsPerSample = 16;
    int subchunk1Size = 16;

    fwrite("RIFF", 1, 4, f);
    fwrite(&chunkSize, 4, 1, f);
    fwrite("WAVEfmt ", 1, 8, f);
    fwrite(&subchunk1Size, 4, 1, f);
    fwrite(&audioFormat, 2, 1, f);
    fwrite(&numChannels, 2, 1, f);
    fwrite(&sampleRate, 4, 1, f);
    fwrite(&byteRate, 4, 1, f);
    fwrite(&blockAlign, 2, 1, f);
    fwrite(&bitsPerSample, 2, 1, f);
    fwrite("data", 1, 4, f);
    fwrite(&dataSize, 4, 1, f);
    fwrite(samples.data(), 2, samples.size(), f);
    fclose(f);
}

inline void ensureAudioFilesExist()
{
    if (audioFileExists(getAudioPath("bgm.wav"))) {
        return; // Already present
    }

#ifdef _WIN32
    _mkdir("audio");
#endif

    const int sr = 44100;
    const double pi = 3.14159265358979323846;

    // 1. select.wav (Crisp UI Click)
    {
        std::vector<short> smp;
        int n = (int)(sr * 0.10);
        smp.reserve(n);
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double env = std::exp(-t * 30.0);
            double v = (std::sin(2 * pi * 1400 * t) + 0.5 * std::sin(2 * pi * 2100 * t)) * env * 0.45;
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/select.wav", smp);
    }

    // 2. deploy.wav (Clash Royale Card Placement Horn)
    {
        std::vector<short> smp;
        int n = (int)(sr * 0.32);
        smp.reserve(n);
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double freq = (t < 0.10) ? 440.0 : ((t < 0.20) ? 554.37 : 659.25);
            double env = (t >= 0.20) ? std::exp(-(t - 0.20) * 8.0) : 0.85;
            double v = (std::sin(2 * pi * freq * t) + 0.35 * std::sin(4 * pi * freq * t)) * env * 0.50;
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/deploy.wav", smp);
    }

    // 3. coin.wav (Gold Coin Sparkle Ding)
    {
        std::vector<short> smp;
        int n = (int)(sr * 0.28);
        smp.reserve(n);
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double env = std::exp(-t * 12.0);
            double v = (std::sin(2 * pi * 1975.5 * t) + 0.6 * std::sin(2 * pi * 2637.0 * t)) * env * 0.45;
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/coin.wav", smp);
    }

    // 4. hit.wav (Sword Clash)
    {
        std::vector<short> smp;
        int n = (int)(sr * 0.18);
        smp.reserve(n);
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double env = std::exp(-t * 30.0);
            double noise = ((i * 1103515245 + 12345) % 1000 - 500) / 500.0;
            double v = (std::sin(2 * pi * 320 * t) * 0.4 + noise * 0.6) * env * 0.55;
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/hit.wav", smp);
    }

    // 5. arrow.wav (Archer Bowstring Snap & Swoosh)
    {
        std::vector<short> smp;
        int n = (int)(sr * 0.20);
        smp.reserve(n);
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double freq = 650.0 - t * 1600.0;
            double env = std::sin(pi * (t / 0.20));
            double v = std::sin(2 * pi * freq * t) * env * 0.40;
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/arrow.wav", smp);
    }

    // 6. death.wav (Character Defeated)
    {
        std::vector<short> smp;
        int n = (int)(sr * 0.28);
        smp.reserve(n);
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double freq = 320.0 * std::exp(-t * 6.0);
            double env = std::exp(-t * 9.0);
            double v = (std::sin(2 * pi * freq * t) + 0.25 * std::sin(6 * pi * freq * t)) * env * 0.45;
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/death.wav", smp);
    }

    // 7. win.wav (3-Crown Victory Horns & Laugh Cadence)
    {
        std::vector<short> smp;
        int n = (int)(sr * 2.0);
        smp.reserve(n);
        struct Note { double s, e, f; };
        Note notes[] = { {0.0, 0.25, 523.25}, {0.25, 0.50, 659.25}, {0.50, 0.75, 783.99}, {0.75, 1.40, 1046.50} };
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double v = 0.0;
            for (int k = 0; k < 4; ++k) {
                if (t >= notes[k].s && t < notes[k].e) {
                    double lt = t - notes[k].s;
                    double env = std::min(1.0, lt * 20.0) * std::exp(-(lt / (notes[k].e - notes[k].s)) * 2.0);
                    v += (std::sin(2 * pi * notes[k].f * t) + 0.4 * std::sin(4 * pi * notes[k].f * t)) * env * 0.50;
                }
            }
            if (t >= 1.40) {
                double lt = t - 1.40;
                double pulse = std::sin(2 * pi * 7.0 * lt);
                if (pulse > 0.0) {
                    v += std::sin(2 * pi * 587.33 * lt) * pulse * 0.38 * std::exp(-lt * 2.0);
                }
            }
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/win.wav", smp);
    }

    // 8. lose.wav (Defeat Horn)
    {
        std::vector<short> smp;
        int n = (int)(sr * 1.2);
        smp.reserve(n);
        struct Note { double s, e, f; };
        Note notes[] = { {0.0, 0.3, 440.0}, {0.3, 0.6, 415.3}, {0.6, 0.9, 392.0}, {0.9, 1.2, 349.2} };
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double v = 0.0;
            for (int k = 0; k < 4; ++k) {
                if (t >= notes[k].s && t < notes[k].e) {
                    double lt = t - notes[k].s;
                    double env = std::exp(-lt * 2.5);
                    v += (std::sin(2 * pi * notes[k].f * t) + 0.25 * std::sin(3 * pi * notes[k].f * t)) * env * 0.45;
                }
            }
            smp.push_back((short)(v * 32767));
        }
        writePcmWav("audio/lose.wav", smp);
    }

    // 9. bgm.wav (Marching Battle Music)
    {
        std::vector<short> smp;
        int n = (int)(sr * 8.0);
        smp.reserve(n);
        struct Note { double s, e, f; };
        Note melody[] = {
            {0.0, 1.0, 261.63}, {1.0, 2.0, 329.63}, {2.0, 3.0, 392.00}, {3.0, 4.0, 523.25},
            {4.0, 5.0, 440.00}, {5.0, 6.0, 392.00}, {6.0, 7.0, 349.23}, {7.0, 8.0, 293.66}
        };
        for (int i = 0; i < n; ++i) {
            double t = (double)i / sr;
            double mv = 0.0;
            for (int k = 0; k < 8; ++k) {
                if (t >= melody[k].s && t < melody[k].e) {
                    double lt = t - melody[k].s;
                    double env = (1.0 - std::exp(-lt * 15.0)) * std::exp(-lt * 0.6);
                    mv = (std::sin(2 * pi * melody[k].f * t) + 0.3 * std::sin(4 * pi * melody[k].f * t)) * env * 0.25;
                }
            }
            double beat = std::fmod(t, 0.50);
            double drum = std::sin(2 * pi * 85.0 * (1.0 - beat * 2.0) * beat) * std::exp(-beat * 18.0) * 0.30;
            smp.push_back((short)((mv + drum) * 32767));
        }
        writePcmWav("audio/bgm.wav", smp);
    }

    std::cout << "[Audio] Sound assets generated successfully in audio/ folder." << std::endl;
}

// -------------------------------------------------------------------------
// Volume Control & Synchronization
// -------------------------------------------------------------------------
inline void updateAudioVolume()
{
#ifdef _WIN32
    DWORD dVol = g_audioMuted ? 0 : (DWORD)((g_audioVolume * 65535) / 100);
    waveOutSetVolume(0, (dVol & 0xFFFF) | ((dVol & 0xFFFF) << 16));

    if (g_audioMuted) {
        mciSendStringA("pause bgm", NULL, 0, NULL);
    } else if (g_audioVolume > 0) {
        mciSendStringA("play bgm repeat", NULL, 0, NULL);
    }
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

// -------------------------------------------------------------------------
// Initialization & Playback
// -------------------------------------------------------------------------
inline void playBGM()
{
#ifdef _WIN32
    if (g_audioMuted || g_audioVolume <= 0) return;
    std::string path = getAudioPath("bgm.wav");
    for (size_t i = 0; i < path.length(); ++i) {
        if (path[i] == '/') path[i] = '\\';
    }

    char fullPath[MAX_PATH];
    if (GetFullPathNameA(path.c_str(), MAX_PATH, fullPath, NULL) != 0) {
        path = fullPath;
    }

    mciSendStringA("close bgm", NULL, 0, NULL);
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "open \"%s\" alias bgm", path.c_str());
    MCIERROR err = mciSendStringA(cmd, NULL, 0, NULL);
    if (err == 0) {
        mciSendStringA("play bgm repeat", NULL, 0, NULL);
    } else {
        char errBuf[256];
        mciGetErrorStringA(err, errBuf, sizeof(errBuf));
        std::cout << "[Audio] BGM open note: " << errBuf << std::endl;
    }
#endif
}

inline void stopBGM()
{
#ifdef _WIN32
    mciSendStringA("stop bgm", NULL, 0, NULL);
#endif
}

inline void initAudio()
{
    ensureAudioFilesExist();
    updateAudioVolume();
    std::cout << "[Audio] Audio system active. Volume: " << g_audioVolume << "%" << std::endl;
}

inline void playAudioFile(const char* filename)
{
    if (g_audioMuted || g_audioVolume <= 0) return;
    std::string path = getAudioPath(filename);
#ifdef _WIN32
    for (size_t i = 0; i < path.length(); ++i) {
        if (path[i] == '/') path[i] = '\\';
    }
    PlaySoundA(path.c_str(), NULL, SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
#else
    (void)path;
#endif
}

inline void playAudioSelect() { playAudioFile("select.wav"); }
inline void playAudioDeploy() { playAudioFile("deploy.wav"); }
inline void playAudioCoin()   { playAudioFile("coin.wav"); }
inline void playAudioHit()    { playAudioFile("hit.wav"); }
inline void playAudioArrow()  { playAudioFile("arrow.wav"); }
inline void playAudioDeath()  { playAudioFile("death.wav"); }
inline void playAudioWin()    { playAudioFile("win.wav"); }
inline void playAudioLose()   { playAudioFile("lose.wav"); }
