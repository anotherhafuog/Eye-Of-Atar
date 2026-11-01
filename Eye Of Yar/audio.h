#pragma once
#if __has_include(<SDL2/SDL_mixer.h>)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#else
#include <SDL.h>
#include <SDL_mixer.h>
#endif

namespace audio {

    inline bool init() {
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0 ||
            Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("Audio init failed: %s", SDL_GetError());
            return false;
        }
        Mix_AllocateChannels(16);
        return true;
    }

    inline Mix_Chunk* loadSfx(const char* path) {
        Mix_Chunk* c = Mix_LoadWAV(path);
        if (!c) SDL_Log("Failed to load %s: %s", path, Mix_GetError());
        return c;
    }
    inline void playSfx(Mix_Chunk* c) { if (c) Mix_PlayChannel(-1, c, 0); }

    inline Mix_Music* loadMusic(const char* path) {
        Mix_Music* m = Mix_LoadMUS(path);
        if (!m) SDL_Log("Failed to load %s: %s", path, Mix_GetError());
        return m;
    }
    inline void playMusic(Mix_Music* m, int loops = -1) { if (m) Mix_PlayMusic(m, loops); }
    inline void stopMusic() { Mix_HaltMusic(); }

    inline void shutdown() {
        Mix_CloseAudio();
        Mix_Quit();
    }

} // namespace audio

//file courtesy of claude