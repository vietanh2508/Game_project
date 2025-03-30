#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>
#include <string>
#include <map>

class Audio {
public:
    Audio(); // Constructor
    ~Audio();// Destructor

    bool Init();  // Khởi tạo SDL_mixer
    void LoadSound(const std::string& name, const std::string& filepath);// Tải âm thanh từ file
    void PlaySound(const std::string& name, int loops = 0);// Phát âm thanh
    void StopAllSounds();// Dừng tất cả các âm thanh đang phát


private:
    std::map<std::string, Mix_Chunk*> sounds;// Lưu trữ các âm thanh đã tải
};

#endif