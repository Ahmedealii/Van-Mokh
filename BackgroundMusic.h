#ifdef _WIN32
#pragma once

// Minimal Windows background-music player for this project.
// Windows MCI can decode MP3 audio without an external library.
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <iostream>

#pragma comment(lib, "winmm.lib")

namespace BackgroundMusic {
    inline bool& isOpen() {
        static bool opened = false;
        return opened;
    }

    inline bool& isPlaying() {
        static bool playing = false;
        return playing;
    }

    inline bool fileExists(const std::string& path) {
        DWORD attrs = GetFileAttributesA(path.c_str());
        return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
    }

    inline std::string findMusicFile() {
        // Working directory normally is the Template project folder in Visual Studio.
        const char* candidates[] = {
            "assets\\audio\\background.mp3",
            "..\\..\\assets\\audio\\background.mp3", // running from x64/Debug
            "Template\\assets\\audio\\background.mp3", // running from repository root
            "..\\Template\\assets\\audio\\background.mp3"
        };
        for (const char* candidate : candidates) {
            if (!fileExists(candidate)) continue;
            char fullPath[MAX_PATH] = {};
            if (GetFullPathNameA(candidate, MAX_PATH, fullPath, nullptr))
                return std::string(fullPath);
        }
        return "";
    }

    inline void logError(const char* action, MCIERROR error) {
        char message[256] = {};
        mciGetErrorStringA(error, message, sizeof(message));
        std::cerr << "Background music " << action << " failed: " << message << '\n';
    }

    inline void stop() {
        if (!isOpen()) return;
        mciSendStringA("stop vanMokhBgm", nullptr, 0, nullptr);
        mciSendStringA("close vanMokhBgm", nullptr, 0, nullptr);
        isOpen() = false;
        isPlaying() = false;
    }

    inline void start() {
        if (isPlaying()) return;
        if (!isOpen()) {
            std::string file = findMusicFile();
            if (file.empty()) {
                std::cerr << "Background music not found: assets/audio/background.mp3\n";
                return;
            }
            std::string command = "open \"" + file + "\" type mpegvideo alias vanMokhBgm";
            MCIERROR error = mciSendStringA(command.c_str(), nullptr, 0, nullptr);
            if (error) { logError("open", error); return; }
            isOpen() = true;
        }
        // 'repeat' tells the Windows MCI MPEG audio device to loop until stopped.
        MCIERROR error = mciSendStringA("play vanMokhBgm from 0 repeat", nullptr, 0, nullptr);
        if (error) {
            logError("play", error);
            stop();
            return;
        }
        isPlaying() = true;
    }
}

#else
namespace BackgroundMusic { inline void start(){} inline void stop(){} }
#endif
