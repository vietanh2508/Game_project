#ifndef PLAYER_H
#define PLAYER_H

#include<SDL.h>
#include<vector>
#include<string>
#include"collision.h"

class Player {
public:
	enum class State { IDLE, RUNNING, JUMPING, FALLING};
		
	Player();
	~Player();

	void LoadSprites(SDL_Renderer* renderer , const std::string& path);
	void Update(float deltaTime, const std::vector<SDL_Rect>& tiles);
	void Render(SDL_Renderer* renderer);

	void MoveLeft();
	void MoveRight();
	void Jump();
	void Stop();

	SDL_Rect GetRect() const { return destRect; }
	void SetFrameSize(int width, int height);
	void SetDisplaySize(int width, int height);

private:
	void UpdateAnimation(float deltaTime);
	void UpdateState();
	
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	State currentState;
	int frameWidth, frameHeight;
	int currentFrame, totalFrames;
	float frameTime, accumulator;
	float velocityX, velocityY;
	bool isFlipped, isOnGround;
	Collision collision;
	float x, y;
};

#endif 
 