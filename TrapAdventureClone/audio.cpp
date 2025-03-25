#include "audio.h"
#include <iostream>

Audio::Audio() {}

Audio::~Audio() {
    StopAllSounds();
    for (auto& it : sounds) {
        Mix_FreeChunk(it.second);
    }
    Mix_CloseAudio();
}

bool Audio::Init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void Audio::LoadSound(const std::string& name, const std::string& filepath) {
    Mix_Chunk* sound = Mix_LoadWAV(filepath.c_str());
    if (!sound) {
        std::cerr << "Failed to load sound effect: " << filepath << " - SDL_Error: " << Mix_GetError() << std::endl;
        return;
    }
    sounds[name] = sound;
}

void Audio::PlaySound(const std::string& name, int loops) {
    if (sounds.find(name) != sounds.end()) {
        Mix_PlayChannel(-1, sounds[name], loops);
    }
    else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void Audio::StopAllSounds() {
    Mix_HaltChannel(-1); // Stop all channels
}