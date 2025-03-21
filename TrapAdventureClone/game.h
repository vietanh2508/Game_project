#ifndef GAME_H
#define GAME_H

#include<SDL.h>
#include"renderer.h"
#include"player.h"
#include"level.h"

class Game {
public :
	Game();
	~Game();

	bool init();
	void run();
	void handleInput();
	void update(float deltaTime);
	void render();

private:
	Level* level;
	SDL_Window* window;
	Renderer renderer;
	bool isRunning;
	Player player;
	const float TARGET_FPS = 60.0f;             
	const Uint32 TARGET_FRAME_TIME = 1000 / 60;   
};

#endif