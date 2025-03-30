#ifndef MENU_H
#define MENU_H

#include <SDL.h>      
#include "renderer.h" 
#include "audio.h"    

class Menu {
public:
    //  Định nghĩa các trạng thái của menu
    enum class State {
        MAIN_MENU, 
        IN_GAME  
    };

    // Constructor: Khởi tạo menu
    Menu(Renderer& renderer, Audio& audio); // Cần Renderer và Audio để hoạt động
    ~Menu();  // Destructor: Giải phóng bộ nhớ

    void HandleEvent(SDL_Event& event); // Xử lý các sự kiện (ví dụ: click chuột)
    void Render(State state);        // Vẽ menu lên màn hình
    void ResetSelection();          // Đặt lại lựa chọn hiện tại
    bool IsPaused() const;          // Kiểm tra xem game có đang tạm dừng không

    // Enum Action: Định nghĩa các hành động có thể xảy ra khi tương tác với menu
    enum class Action {
        NONE,  
        PLAY,  
        EXIT,  
        PAUSE 
    };
    Action GetAction() const { return currentAction; } // Lấy hành động hiện tại

private:
    Renderer& renderer; // Tham chiếu đến đối tượng Renderer
    Audio& audio;     // Tham chiếu đến đối tượng Audio
    Action currentAction; // Hành động hiện tại mà người chơi đã chọn

    // Các texture cho các thành phần của menu
    SDL_Texture* playTexture;
    SDL_Texture* exitTexture;
    SDL_Texture* pauseTexture;
    SDL_Texture* backgroundTexture;

    // Các hình chữ nhật định vị các button
    SDL_Rect playButton;
    SDL_Rect exitButtonMain;
    SDL_Rect pauseButton;
    SDL_Rect exitButtonInGame;

    bool IsMouseOver(const SDL_Rect& rect) const; // Kiểm tra xem chuột có đang nằm trên một hình chữ nhật không
    bool isPaused = false; // Biến để theo dõi trạng thái tạm dừng của game
};

#endif