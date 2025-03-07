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
	void handInput();
	void update(float deltaTime);
	void render();

private:
	Level level;
	SDL_Window* window;
	Renderer renderer;
	bool isRunning;
	Player player;
};

#endif