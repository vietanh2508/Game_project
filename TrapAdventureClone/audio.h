#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>
#include <string>
#include <map>

class Audio {
public:
    Audio();
    ~Audio();

    bool Init(); // Initialize SDL_mixer
    void LoadSound(const std::string& name, const std::string& filepath);
    void PlaySound(const std::string& name, int loops = 0);
    void StopAllSounds();

private:
    std::map<std::string, Mix_Chunk*> sounds;
};

#endif