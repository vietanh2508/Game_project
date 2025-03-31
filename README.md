Họ và tên : Vũ Việt Anh 
MSV : 24020033
Chủ đề game : Đây là bản sao Trap Adventure, nơi người chơi điều hướng một màn chơi nguy hiểm đầy bẫy, cố gắng đến đích.
Ngưỡng điểm : 8.5
Lý do bảo vệ :
Game có các tính năng tự làm , tự học sau :
*   **Hệ thống va chạm AABB:** Tự cài đặt thuật toán va chạm AABB (Axis-Aligned Bounding Box) để phát hiện va chạm giữa player và các tiles, traps.
*   **Triển khai hành vi bẫy:**
    *   SHOOT_LEFT, SHOOT_RIGHT: Bẫy bắn theo chiều ngang sau khi kích hoạt.
        *   SLIDE_LEFT, SLIDE_RIGHT: Bẫy trượt sang trái hoặc phải.
        *   FALL: Bẫy rơi sau khi kích hoạt.
*   **Thiết kế màn chơi:** Tôi đã sửa đổi đáng kể bố cục màn chơi bằng cách sử dụng các tệp CSV và phần mềm chỉnh sửa ô gạch Tiled, kết hợp các loại bẫy mới và thay đổi địa hình.
*   **Quản lý trạng thái và Animation:** Tự thiết kế hệ thống quản lý trạng thái (đứng yên, chạy, nhảy, rơi) và animation cho nhân vật người chơi.
*   **Điều khiển âm thanh:** Sử dụng SDL_mixer để tải và phát các hiệu ứng âm thanh, tạo cảm giác sống động cho game.
*   **Thiết kế level "troll":** Học hỏi cách thiết kế level bất ngờ và khó đoán từ game gốc "Trap Adventure 2".
Các nguồn tham khảo :
*   **Âm thanh:** sử dụng Developer Tools (F12) của trình duyệt để tải một số hiệu ứng âm thanh từ trang: https://www.gamepix.com/play/trap-adventure-2
*   **Hình ảnh player,wall,trap:** :Tự làm từ trang web : https://www.pixilart.com/
*   **Ảnh victory:**  https://www.pinterest.com/
Mức độ sử dụng AI:(60%)
*   Sử dụng google AI để tạo khung chương trình
*   Sử dụng AI để gỡ lỗi
*   Sử dụng AI để tạo mã cơ bản 
