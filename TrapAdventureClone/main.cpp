#include<SDL.h>
#include<iostream>
#include"game.h"

int main(int agrc, char* agrv[]) {
	Game game;

    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1; 
    }

    game.run();
    return 0;
}