#ifndef PLAYER_H
#define PLAYER_H

#include<SDL.h>
#include<vector>
#include<string>

class Player {
public:
	enum class State { IDLE, RUNNING, JUMPING, FALLING};
		
	Player();
	~Player();

	void LoadSprites(SDL_Renderer* renderer , const std::string& path);
	void Update(float deltaTime);
	void Render(SDL_Renderer* renderer);

	void MoveLeft();
	void MoveRight();
	void Jump();
	void Stop();

	SDL_Rect GetRect() const { return destRect; }
	void SetFrameSize(int width, int height);
	void SetDisplaySize(int width, int height);
	void CheckGroundCollision(const std::vector<SDL_Rect>& groundTiles);

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
		
};

#endif 
 