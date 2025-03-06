#include"renderer.h"
#include<SDL.h>

Renderer::Renderer() : renderer(nullptr) {};

Renderer::~Renderer() {
	ShutDown();
}

bool Renderer::init(SDL_Window* window) {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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