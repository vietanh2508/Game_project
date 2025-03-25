#ifndef PLAYER_H
#define PLAYER_H

#include<SDL.h>
#include<vector>
#include<string>
#include "collision.h"
#include "trap.h"
#include "audio.h"  // Include Audio header

class Player {
public:
    enum class State { RUNNING, JUMPING, FALLING, IDLE };

    Player(Audio& audio);
    ~Player();

    void LoadSprites(SDL_Renderer* renderer, const std::string& path);
    void Update(float deltaTime, const std::vector<SDL_Rect>& tiles, const std::vector<Trap>& traps);
    void Render(SDL_Renderer* renderer);

    void MoveLeft();
    void MoveRight();
    void Jump();
    void Stop();

    SDL_Rect GetRect() const { return destRect; }
    void SetFrameSize(int width, int height);
    void SetDisplaySize(int width, int height);
    bool isAlive;
private:
    void UpdateAnimation(float deltaTime);
    void UpdateState();

    Collision collision;
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect destRect;
    State currentState;
    int frameWidth, frameHeight;
    int currentFrame, totalFrames;
    float frameTime, accumulator;
    float velocityX, velocityY;
    bool isFlipped, isOnGround;
    Audio& audio;  
};

#endif