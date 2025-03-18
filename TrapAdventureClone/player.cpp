#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include "player.h"
#include "collision.h"
#include "level.h"

Player::Player()
    : texture(nullptr), currentState(State::IDLE),
    frameWidth(16), frameHeight(16),
    currentFrame(0), totalFrames(4),
    frameTime(0.2f), accumulator(0.0f),
    velocityX(0), velocityY(0), collision(),
    isFlipped(false), isOnGround(false) , x(0) , y(0) {
    destRect = { 0, 0, 16, 16 };
    srcRect = { 0, 0, 16, 16 };
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

void Player::Update(float deltaTime, const std::vector<SDL_Rect>& tiles) {
    velocityY += 1200.0f * deltaTime;
    
    float newX = x + velocityX * deltaTime;
    float newY = y + velocityY * deltaTime;

    SDL_Rect tempRect = {
        static_cast<int>(newX),
        static_cast<int>(newY),
        destRect.w = 16,
        destRect.h = 16
    };

    isOnGround = false;

    collision.HandleCollisions(tempRect, velocityX, velocityY, isOnGround, tiles);

    x += velocityX * deltaTime; 
    y += velocityY * deltaTime;

    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);

    UpdateState();
    UpdateAnimation(deltaTime);
}

void Player::UpdateState() {
    State newState = currentState;

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
        frameTime = 0.06f;
        totalFrames = 4;
        break;
    case State::JUMPING:
        srcRect.y = frameHeight * 1;
        totalFrames = 1;
        break;
    case State::FALLING:
        srcRect.y = frameHeight * 2;
        totalFrames = 1;
        break;
    case State::IDLE:
        srcRect.y = frameHeight * 3;
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
    velocityX = -200.0f;
    isFlipped = true;
}

void Player::MoveRight() {
    velocityX = 200.0f;
    isFlipped = false;
}

void Player::Jump() {
    if (isOnGround) {
        velocityY = -500.0f;
        isOnGround = false;
    }
}

void Player::Stop() {
    velocityX = 0;
}

