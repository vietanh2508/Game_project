#ifndef RENDERER_H
#define RENDERER_H

#include<SDL.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	bool init(SDL_Window* window);
	void ShutDown();
	void Clear();
	void Present();


	bool init();
private:
	SDL_Renderer* renderer;
};


#endif