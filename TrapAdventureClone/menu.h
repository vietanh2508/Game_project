#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "renderer.h"

class Menu {
public:
    enum class State {
        MAIN_MENU,    
        IN_GAME      
    };

    Menu(Renderer& renderer);
    ~Menu();

    void HandleEvent(SDL_Event& event);
    void Render(State state);
    void ResetSelection();
    bool IsPaused() const;

    enum class Action {
        NONE,
        PLAY,
        EXIT,
        PAUSE
    };
    Action GetAction() const { return currentAction; }

private:
    Renderer& renderer;
    Action currentAction;

    SDL_Texture* playTexture;
    SDL_Texture* exitTexture;
    SDL_Texture* pauseTexture;
    SDL_Texture* backgroundTexture;

    SDL_Rect playButton;
    SDL_Rect exitButtonMain;
    SDL_Rect pauseButton;
    SDL_Rect exitButtonInGame;
    bool IsMouseOver(const SDL_Rect& rect) const;
    bool isPaused = false;
};

#endif