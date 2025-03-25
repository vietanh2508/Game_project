#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <algorithm>
#include "player.h"
#include "collision.h"
#include "level.h"
#include "trap.h"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

Player::Player()
    : texture(nullptr), currentState(State::IDLE),
    frameWidth(32), frameHeight(32),
    currentFrame(0), totalFrames(4),
    frameTime(1.0f), accumulator(0.0f),
    velocityX(0), velocityY(0), collision(),
    isFlipped(false), isOnGround(false), isAlive(true) {
    destRect = { 0, 16, 32, 32 };
    srcRect = { 0, 0, 32, 32 };
}

Player :: ~Player() {
    if (texture) SDL_DestroyTexture(texture);
}

void Player::LoadSprites(SDL_Renderer* renderer, const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
}

void Player::SetFrameSize(int width, int height) {
    frameWidth = width;
    frameHeight = height;
}

void Player::SetDisplaySize(int width, int height) {
    destRect.w = width;
    destRect.h = height;
}

void Player::Update(float deltaTime, const std::vector<SDL_Rect>& tiles , const std::vector<Trap>& traps) {
    if (!isAlive) return;
    velocityY += 900.0f * deltaTime;
    
    float newX = destRect.x + velocityX * deltaTime;
    float newY = destRect.y + velocityY * deltaTime;

    SDL_Rect tempRect = {
        static_cast<int>(newX),
        static_cast<int>(newY),
        destRect.w ,
        destRect.h
    };

    isOnGround = false;
    collision.HandleCollisions(tempRect, velocityX, velocityY, isOnGround, tiles);
    for (const auto& trap : traps) {
        if (collision.CheckCollision(destRect, trap.GetRect())) {
            isAlive = false;
            velocityX = 0; 
            velocityY = 0;
            break;
        }
    }

    destRect.x = tempRect.x;
    destRect.y = tempRect.y;

    if (isAlive) {
        if (destRect.x < 0) destRect.x = 0;
        if (destRect.x + destRect.w > SCREEN_WIDTH) destRect.x = SCREEN_WIDTH - destRect.w;
        if (destRect.y < 0) destRect.y = 0;
    }

    UpdateState();
    UpdateAnimation(deltaTime);
}        

void Player::UpdateState() {
    State newState;

    if (isOnGround) {
        newState = (velocityX != 0) ? State::RUNNING : State::IDLE;
    }
    else {
        newState = (velocityY < 0) ? State::JUMPING : State::FALLING;
    }

    if (newState != currentState) {
        currentFrame = 0;
        accumulator = 0.0f;
        currentState = newState;
    }
}

void Player::UpdateAnimation(float deltaTime) {
    accumulator += deltaTime;

    switch (currentState) {
    case State::RUNNING:
        srcRect.y = 0;       
        frameTime = 0.1f;    
        totalFrames = 4;     
        break;
    case State::JUMPING:
        srcRect.y = 32;    
        totalFrames = 1;
        break;
    case State::FALLING:
        srcRect.y = 64;      
        totalFrames = 1;
        break;
    case State::IDLE:
        srcRect.y = 96;      
        totalFrames = 1;
        break;
    }
                                  
    if (accumulator >= frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames;
        accumulator = 0.0f;
    }
    srcRect.x = currentFrame * frameWidth;
}

void Player::Render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RendererFlip flip = isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0.0, nullptr, flip);
    }
}

void Player::MoveLeft() {
    if (isAlive) {
        velocityX = -200.0f;
    }
    isFlipped = true;
}

void Player::MoveRight() {
    if (isAlive) {
        velocityX = 200.0f;
    }
    isFlipped = false;
}

void Player::Jump() {
    if (isOnGround && isAlive) {
        velocityY = -450.0f;
        isOnGround = false;
    }
}

void Player::Stop() {
    velocityX = 0;
}

