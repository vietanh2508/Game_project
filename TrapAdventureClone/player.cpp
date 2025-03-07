#include"player.h"
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>

Player::Player()
    : texture(nullptr), currentState(State::IDLE),
    frameWidth(32), frameHeight(32),
    currentFrame(0), totalFrames(4),
    frameTime(0.1f), accumulator(0.0f),
    velocityX(0), velocityY(0),
    isFlipped(false), isOnGround(false) {
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

void Player::Update(float deltaTime) {
    destRect.x += static_cast<int>(velocityX * deltaTime);
    destRect.y += static_cast<int>(velocityY * deltaTime);

    if (!isOnGround) velocityY += 980.0f * deltaTime;
    if (velocityY > 1000.0f) velocityY = 1000.0f;

    UpdateState();
    UpdateAnimation(deltaTime);
}

void Player::UpdateState() {
    State previousState = currentState;

    if (velocityY < 0) {
        currentState = State::JUMPING;
    }
    else if (velocityY > 0 && !isOnGround) {
        currentState = State::FALLING;
    }
    else if (velocityX != 0) {
        currentState = State::RUNNING;
    }
    else {
        currentState = State::IDLE;
    }

    if (currentState != previousState) {
        currentFrame = 0;
        accumulator = 0.0f;
    }
}

void Player::UpdateAnimation(float deltaTime) {
    accumulator += deltaTime;
    if (accumulator >= frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames;
        accumulator = 0.0f;
    }

    switch (currentState) {
    case State::RUNNING:
        srcRect.y = 0;
        totalFrames = 4;
        break;
    case State::JUMPING:
        srcRect.y = frameHeight;
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
    srcRect.x = currentFrame * frameWidth;
    srcRect.w = frameWidth;
    srcRect.h = frameHeight;
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
        velocityY = -400.0f;
        isOnGround = false;
    }
}

void Player::Stop() {
    velocityX = 0;
}

void Player::CheckGroundCollision(const std::vector<SDL_Rect>& groundTiles) {
    isOnGround = false;
    SDL_Rect feet = {
        destRect.x,
        destRect.y + destRect.h - 5, 
        destRect.w,
        10
    };

    for (const auto& tile : groundTiles) {
        if (SDL_HasIntersection(&feet, &tile)) {
            isOnGround = true;
            destRect.y = tile.y - destRect.h;
            velocityY = 0;
            break;
        }
    }
}
