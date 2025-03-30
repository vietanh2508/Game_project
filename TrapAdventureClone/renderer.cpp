#include<SDL_image.h>
#include<iostream>
#include"renderer.h"

Renderer::Renderer() : renderer(nullptr) {};   // Constructor: Khởi tạo renderer = nullptr

Renderer::~Renderer() { // Destructor: Giải phóng bộ nhớ khi Renderer bị hủy
    if (renderer) {
        SDL_DestroyRenderer(renderer); // Giải phóng renderer của SDL2
        renderer = nullptr;
    }
}

bool Renderer::init(SDL_Window* window) {// Khởi tạo renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    return true;
}

void Renderer::Clear() {
    SDL_RenderClear(renderer); // Xóa màn hình với màu đã đặt bằng SDL_SetRenderDrawColor   
}

void Renderer::Present() {
    SDL_RenderPresent(renderer); // Đẩy những gì đã vẽ lên framebuffer để hiển thị
}

SDL_Texture* Renderer::LoadTexture(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str()); // Tải ảnh vào một SDL_Surface (dữ liệu ảnh ở CPU)
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);// Tạo texture từ SDL_Surface 
    SDL_FreeSurface(surface); // Giải phóng SDL_Surface 
    if (!texture) {
        std::cerr << "Lỗi tạo texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& destRect, bool flip) { // Vẽ texture lên màn hình
    SDL_RendererFlip flipType = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE; // Xác định kiểu flip (lật ngang)
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0.0, nullptr, flipType);
}