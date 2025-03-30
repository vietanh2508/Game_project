#include <iostream>    
#include <SDL_image.h>  
#include "menu.h"      

// Constructor: Khởi tạo menu
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

// Destructor: Giải phóng bộ nhớ
Menu::~Menu() {
    if (playTexture) SDL_DestroyTexture(playTexture);  
    if (exitTexture) SDL_DestroyTexture(exitTexture);  
    if (pauseTexture) SDL_DestroyTexture(pauseTexture); 
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture); 
}

// Xử lý các sự kiện
void Menu::HandleEvent(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) { 
        // Kiểm tra xem chuột có click vào button nào không
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

// Vẽ menu lên màn hình
void Menu::Render(State state) {
    if (state == State::MAIN_MENU) { // Nếu đang ở menu chính
        SDL_Rect backgroundRect = { 0, 0, 960, 640 };
        renderer.RenderTexture(backgroundTexture, SDL_Rect{ 0, 0, 960, 640 }, backgroundRect);
        renderer.RenderTexture(playTexture, SDL_Rect{ 0,0,273,36 }, playButton);  // Vẽ play button
        renderer.RenderTexture(exitTexture, SDL_Rect{ 0,0,160,80 }, exitButtonMain);   // Vẽ exit button
    }
    else { // Nếu đang ở menu trong game
        renderer.RenderTexture(pauseTexture, SDL_Rect{ 0,0,64,64 }, pauseButton); // Vẽ pause button
        renderer.RenderTexture(exitTexture, SDL_Rect{ 0,0,64,64 }, exitButtonInGame); // Vẽ exit button
    }
}

// Đặt lại lựa chọn hiện tại
void Menu::ResetSelection() {
    currentAction = Action::NONE; // Đặt hành động về NONE
}

//Kiểm tra xem chuột có đang nằm trên một hình chữ nhật không
bool Menu::IsMouseOver(const SDL_Rect& rect) const {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY); // Lấy vị trí chuột
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w && // Kiểm tra xem chuột có nằm trong hình chữ nhật không
        mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

//  Kiểm tra xem game có đang tạm dừng không
bool Menu::IsPaused() const {
    return isPaused;
}