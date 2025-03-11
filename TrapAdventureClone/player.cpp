#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "player.h"
#include "collision.h"
#include "level.h"

Player::Player()
    : texture(nullptr), currentState(State::IDLE),
    frameWidth(32), frameHeight(32),
    currentFrame(0), totalFrames(4),
    frameTime(0.2f), accumulator(0.0f),
    velocityX(0), velocityY(0), collision(),
    isFlipped(false), isOnGround(false) , x(100) , y(100) {
    destRect = { 100, 100, 64, 64 };
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

void Player::Update(float deltaTime, const std::vector<SDL_Rect>& tiles)    {
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    destRect.x = static_cast<int>(x);
    destRect.y = static_cast<int>(y);

    if (!isOnGround) velocityY += 2000.0f * deltaTime;

    isOnGround = false;

    collision.HandleCollisions(destRect, velocityX, velocityY, isOnGround, tiles);

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
    velocityX = -300.0f;
    isFlipped = true;
}

void Player::MoveRight() {
    velocityX = 300.0f;
    isFlipped = false;
}

void Player::Jump() {
    if (isOnGround) {
        velocityY = -400.0f;
        isOnGround = false;
    }
}

void Player::Stop() {
    velocityX = 0;
}

