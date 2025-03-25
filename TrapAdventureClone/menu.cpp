#include <iostream>
#include <SDL_image.h>
#include "menu.h"

Menu::Menu(Renderer& renderer, Audio& audio)
    : renderer(renderer), currentAction(Action::NONE),
    playTexture(nullptr), exitTexture(nullptr), pauseTexture(nullptr),
    backgroundTexture(nullptr), audio(audio)
    , playButton({ 300, 300, 273, 36 }),
    exitButtonMain({ 20, 556, 64, 64 }),
    pauseButton({ 880, 20, 64, 64 }),
    exitButtonInGame({ 800, 20, 64, 64 })
{
    playTexture = renderer.LoadTexture("assets/image/play.png");
    exitTexture = renderer.LoadTexture("assets/image/exit.png");
    pauseTexture = renderer.LoadTexture("assets/image/pause.png");
    backgroundTexture = renderer.LoadTexture("assets/image/background.png");
}

Menu::~Menu() {
    if (playTexture) SDL_DestroyTexture(playTexture);
    if (exitTexture) SDL_DestroyTexture(exitTexture);
    if (pauseTexture) SDL_DestroyTexture(pauseTexture);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
}

void Menu::HandleEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (IsMouseOver(playButton)) {
            audio.PlaySound("button_click");
            currentAction = Action::PLAY;
        }
        else if (IsMouseOver(exitButtonMain)) {
            audio.PlaySound("button_click");
            currentAction = Action::EXIT;
        }
        else if (IsMouseOver(pauseButton)) {
            audio.PlaySound("button_click");
            isPaused = !isPaused;
            currentAction = Action::PAUSE;
        }
        else if (IsMouseOver(exitButtonInGame)) {
            audio.PlaySound("button_click");
            currentAction = Action::EXIT;
        }
    }
}

void Menu::Render(State state) {
    if (state == State::MAIN_MENU) {
        SDL_Rect backgroundRect = { 0, 0, 960, 640 };
        renderer.RenderTexture(backgroundTexture, SDL_Rect{ 0, 0, 960, 640 }, backgroundRect);
        renderer.RenderTexture(playTexture, SDL_Rect{ 0,0,160,80 }, playButton);
        renderer.RenderTexture(exitTexture, SDL_Rect{ 0,0,160,80 }, exitButtonMain);
    }
    else {
        renderer.RenderTexture(pauseTexture, SDL_Rect{ 0,0,64,64 }, pauseButton);
        renderer.RenderTexture(exitTexture, SDL_Rect{ 0,0,64,64 }, exitButtonInGame);
    }
}

void Menu::ResetSelection() {
    currentAction = Action::NONE;
}

bool Menu::IsMouseOver(const SDL_Rect& rect) const {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
        mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

bool Menu::IsPaused() const {
    return isPaused;
}