#include "audio.h"
#include <iostream> 

Audio::Audio() {} // Constructor: Không làm gì cả 

Audio::~Audio() { // Destructor: Giải phóng bộ nhớ và đóng SDL_mixer
    StopAllSounds(); 
    for (auto& it : sounds) { 
        Mix_FreeChunk(it.second); 
    }
    Mix_CloseAudio(); // Đóng SDL_mixer
}

bool Audio::Init() { // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_Error: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void Audio::LoadSound(const std::string& name, const std::string& filepath) { // Tải âm thanh từ file
    Mix_Chunk* sound = Mix_LoadWAV(filepath.c_str()); 
    if (!sound) {
        std::cerr << "Failed to load sound effect: " << filepath << " - SDL_Error: " << Mix_GetError() << std::endl;
        return;
    }
    sounds[name] = sound; // Lưu Mix_Chunk vào map sounds với tên tương ứng
}

void Audio::PlaySound(const std::string& name, int loops) { // Phát âm thanh
    if (sounds.find(name) != sounds.end()) { 
        Mix_PlayChannel(-1, sounds[name], loops); 
    }
    else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void Audio::StopAllSounds() { // Dừng tất cả các âm thanh
    Mix_HaltChannel(-1); 
}