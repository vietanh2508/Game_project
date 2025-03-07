#ifndef RENDERER_H
#define RENDERER_H

#include<SDL.h>
#include<string>

class Renderer {
public:
	Renderer();
	~Renderer();

	bool init(SDL_Window* window);
	void ShutDown();
	void Clear();
	void Present();
	SDL_Texture* LoadTexture(const std::string& path);
	void RenderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& destRect, bool flip = false);

	SDL_Renderer* GetSDLRenderer() const { return renderer; }
private:
	SDL_Renderer* renderer;
};


#endif