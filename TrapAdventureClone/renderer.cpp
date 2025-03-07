#include"renderer.h"
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>

Renderer::Renderer() : renderer(nullptr) {};

Renderer::~Renderer() {
	ShutDown();
}

bool Renderer::init(SDL_Window* window) {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    return true;
}

void Renderer::ShutDown() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}

void Renderer::Clear() {
    SDL_RenderClear(renderer);
}

void Renderer::Present() {
    SDL_RenderPresent(renderer);
}

SDL_Texture* Renderer::LoadTexture(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Renderer::RenderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& destRect, bool flip) {
    SDL_RendererFlip flipType = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0.0, nullptr, flipType);
}