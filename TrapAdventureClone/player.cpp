#include <SDL_image.h>  
#include <SDL_mixer.h>  
#include <iostream>     
#include <algorithm>  
#include "player.h"    
#include "collision.h"  
#include "level.h"     
#include "trap.h"      

// Kích thước màn hình 
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

// Constructor
Player::Player(Audio& audio)
    : texture(nullptr), currentState(State::IDLE), 
    frameWidth(32), frameHeight(32),            
    currentFrame(0), totalFrames(4),           
    frameTime(0.1f), accumulator(0.0f),           
    velocityX(0), velocityY(0), collision(),   
    lives(10), livesTexture(nullptr),             
    isFlipped(false), isOnGround(false), isAlive(true), audio(audio) { 
    livesSrcRect = { 0, 0, 0, 0 }; 
    destRect = { 0, 16, 32, 32 };   
    srcRect = { 0, 0, 32, 32 };  
}

// Destructor
Player::~Player() {
    if (texture) SDL_DestroyTexture(texture);
    if (livesTexture) SDL_DestroyTexture(livesTexture); 
}

// Tải spritesheet
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

    // Load spritesheet cho lives
    SDL_Surface* livesSurface = IMG_Load("assets/image/lives_spritesheet.png");
    if (!livesSurface) {
        std::cerr << "Failed to load lives spritesheet: " << IMG_GetError() << std::endl;
        return;
    }
    livesTexture = SDL_CreateTextureFromSurface(renderer, livesSurface);
    SDL_FreeSurface(livesSurface);
    if (!livesTexture) {
        std::cerr << "Failed to create lives texture: " << SDL_GetError() << std::endl;
    }

    livesSrcRect = { 0, 0, 32, 32 }; // Lấy frame đầu tiên từ spritesheet lives
}

// Đặt kích thước của một frame trong spritesheet
void Player::SetFrameSize(int width, int height) {
    frameWidth = width;
    frameHeight = height;
}

// Đặt kích thước hiển thị của player
void Player::SetDisplaySize(int width, int height) {
    destRect.w = width;
    destRect.h = height;
}

void Player::Update(float deltaTime, const std::vector<SDL_Rect>& tiles, const std::vector<Trap>& traps) {
    if (!isAlive) return; // Nếu player đã chết, không cập nhật nữa

    // Áp dụng trọng lực
    velocityY += 900.0f * deltaTime;

    // Tính toán vị trí mới
    float newX = destRect.x + velocityX * deltaTime;
    float newY = destRect.y + velocityY * deltaTime;

    // Tạo một hình chữ nhật tạm thời để kiểm tra va chạm
    SDL_Rect tempRect = {
        static_cast<int>(newX),
        static_cast<int>(newY),
        destRect.w ,
        destRect.h
    };

    collision.HandleCollisions(tempRect, velocityX, velocityY, isOnGround, tiles); // Kiểm tra va chạm với các tile

    bool hitTrap = false;
    for (const auto& trap : traps) { // Duyệt qua tất cả các trap
        if (collision.CheckCollision(destRect, trap.GetRect()) && !hitTrap) {
            hitTrap = true; //Đánh dấu là đã dính bẫy để không bị trừ máu nhiều lần
            lives--; // Mất một mạng

            if (lives > 0) {
                isAlive = true;
            }
            else {
                isAlive = false; 
                audio.PlaySound("death"); 
            }
            velocityX = 0; 
            velocityY = 0; 
            break;
        }
    }

    // Cập nhật vị trí thực tế của player
    destRect.x = tempRect.x;
    destRect.y = tempRect.y;

    // Giới hạn vị trí của player trong màn hình
    if (isAlive) { 
        if (destRect.x < 0) destRect.x = 0; // Không cho player đi ra khỏi bên trái màn hình
        if (destRect.x + destRect.w > SCREEN_WIDTH) destRect.x = SCREEN_WIDTH - destRect.w; // Không cho player đi ra khỏi bên phải màn hình
        if (destRect.y < 0) destRect.y = 0; // Không cho player đi ra khỏi bên trên màn hình
    }
    UpdateState(); //Cập nhật trạng thái
    UpdateAnimation(deltaTime); //Cập nhật animation
}

// Cập nhật trạng thái của player
void Player::UpdateState() {
    State newState; // Trạng thái mới

    if (isOnGround) { // Nếu đang chạm đất
        newState = (velocityX != 0) ? State::RUNNING : State::IDLE;
    }
    else { // Nếu không chạm đất
        newState = (velocityY < 0) ? State::JUMPING : State::FALLING; 
    }

    // Nếu trạng thái mới khác với trạng thái hiện tại
    if (newState != currentState) {
        currentFrame = 0; // Reset frame
        accumulator = 0.0f; // Reset accumulator
        currentState = newState; // Cập nhật trạng thái
    }
}

// Cập nhật animation dựa trên trạng thái
void Player::UpdateAnimation(float deltaTime) {
    accumulator += deltaTime; // Cộng dồn thời gian

    switch (currentState) { // Dựa vào trạng thái để chọn animation
    case State::RUNNING:
        srcRect.y = 0;    
        frameTime = 0.1f; 
        totalFrames = 4;  
        if (!Mix_Playing(-1)) 
        {
            audio.PlaySound("run"); 
        }
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

    // Nếu đã đến thời điểm chuyển frame
    if (accumulator >= frameTime) {
        currentFrame = (currentFrame + 1) % totalFrames; // Chuyển sang frame tiếp theo 
        accumulator = 0.0f; // Reset accumulator
    }
    srcRect.x = currentFrame * frameWidth; // Tính toán vị trí X của frame hiện tại trong spritesheet
}

void Player::Render(SDL_Renderer* renderer) {
    if (texture) { 
        SDL_RendererFlip flip = isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE; // Lật hình nếu cần
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0.0, nullptr, flip); // Vẽ player
    }
    // Vẽ số mạng sống còn lại
    if (livesTexture) {
        for (int i = 0; i < lives; ++i) {  // Vẽ từng icon mạng sống
            SDL_Rect livesDestRect = { 10 + i * 40, 10, 32, 32 }; // Vị trí của icon
            livesSrcRect.x = i * 32; // Lấy frame tương ứng với số mạng sống còn lại
            SDL_RenderCopy(renderer, livesTexture, &livesSrcRect, &livesDestRect); // Vẽ icon
        }
    }
}

// Di chuyển player sang trái
void Player::MoveLeft() {
    if (isAlive) { 
        velocityX = -200.0f; 
    }
    isFlipped = true; // Quay mặt sang trái
}

// Di chuyển player sang phải
void Player::MoveRight() {
    if (isAlive) { 
        velocityX = 200.0f; 
    }
    isFlipped = false; // Quay mặt sang phải
}

// Nhảy
void Player::Jump() {
    if (isOnGround && isAlive) { 
        velocityY = -420.0f; 
        isOnGround = false;
        audio.PlaySound("jump");
    }
}

// Dừng di chuyển
void Player::Stop() {
    velocityX = 0; 
}

// Đặt lại vị trí của player về ban đầu
void Player::ResetPosition() {
    destRect.x = 0; 
    destRect.y = 16;
    velocityX = 0;  
    velocityY = 0;  
}