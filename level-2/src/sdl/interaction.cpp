/**
 * SDL Interaction cpp implementation
 *
 */

#include "interaction.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

/* ---------- Definitions ---------- */

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 *   - e: SDL_Event hiện tại.
 * Đầu ra:
 *   - false nếu không phải quit event.
 * Tác dụng phụ:
 *   - Ném QuitException nếu nhận được sự kiện thoát.
 */
bool SDLInteraction::waitForQuit(SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        throw QuitException();
    }
    return false;
}

/**
 * Mô tả: Khởi tạo đối tượng SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::SDLInteraction() {
}

/**
 * Mô tả: Destructor của SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::~SDLInteraction() {
}

/**
 * Mô tả: Khởi tạo hệ thống interaction cho SDL.
 * Đầu vào:
 *   - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Thiết lập trạng thái ban đầu cho input SDL.
 */
void SDLInteraction::init(const RunConfig& config) {
    SDL_StopTextInput();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}
std::string SDLInteraction::readNumberInput() {
    std::string input;
    SDL_StartTextInput();
    SDL_Event event;
 
    while (true) {
        if (!SDL_WaitEvent(&event)) continue;
 
        if (event.type == SDL_QUIT) {
            SDL_StopTextInput();
            throw QuitException();
        }
 
        if (event.type == SDL_TEXTINPUT) {
            if (std::isdigit((unsigned char)event.text.text[0]))
                input += event.text.text;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    if (!input.empty()) {
                        SDL_StopTextInput();
                        return input;
                    }
                    break;
                case SDLK_BACKSPACE:
                    if (!input.empty()) input.pop_back();
                    break;
                case SDLK_ESCAPE:
                    SDL_StopTextInput();
                    return "";
                default:
                    break;
            }
        }
    }
}
/**
 * Mô tả: Tạm dừng chương trình trong SDL.
 *        - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 *        - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 *   - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Có thể block thread hiện tại.
 *   - Có thể ném QuitException nếu người dùng đóng cửa sổ.
 */
void SDLInteraction::pause(int timeout) {
    // Nếu có timeout cụ thể -> delay trực tiếp bằng SDL
    if (timeout > 0) {
        SDL_Delay(timeout);
        return;
    }

    bool waiting = true;
    SDL_Event event;

    // Vòng lặp chờ sự kiện từ người dùng
    while (waiting) {
        // SDL_WaitEvent sẽ block cho tới khi có event
        if (SDL_WaitEvent(&event)) {
            // Nếu người dùng đóng cửa sổ -> thoát game
            if (waitForQuit(event)) {
            }

            // tương tự
            // if (event.type == SDL_QUIT) {
            //     throw QuitException();
            // }

            // Nếu có tương tác (nhấn phím hoặc click chuột) -> kết thúc pause
            if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                waiting = false;
            }
        }
    }
}

/**
 * Mô tả: Lấy input kích thước bàn cờ từ người dùng qua SDL.
 * Đầu vào:
 *   - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 *   - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật giá trị tại size nếu thành công.
 */
bool SDLInteraction::selectSize(int* size) {
     std::string input = readNumberInput();
    if (input.empty()) return false;
 
    try {
        int val = std::stoi(input);
        if (val < BOARD_N_MIN || val > BOARD_N_MAX) return false;
        *size = val;
        return true;
    } catch (...) { return false; }
}

/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 *   - goal: con trỏ lưu giá trị goal.
 *   - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật goal nếu hợp lệ.
 */
bool SDLInteraction::selectGoal(int* goal, const int size) {
     std::string input = readNumberInput();
    if (input.empty()) return false;
 
    try {
        int val = std::stoi(input);
        if (val < 3 || val > GOAL_MAX) return false;
        *goal = val;
        return true;
    } catch (...) { 
        return false; 
    }
    return false;
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 *   - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    SDL_Event event;
    while (true) {
        if (!SDL_WaitEvent(&event)) continue;
 
        if (event.type == SDL_QUIT) throw QuitException();
 
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_1: *mode = GameMode::PVP; return true;
                case SDLK_2: *mode = GameMode::PVE; return true;
                case SDLK_3: *mode = GameMode::EVE; return true;
                case SDLK_ESCAPE: return false;
                default: break;
            }
        }
    }
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 *   - levels: mảng lưu cấp độ bot.
 *   - index: vị trí bot cần gán.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
        if (index < 0 || index > 1) return false;
 
    SDL_Event event;
 
    while (true) {
        if (!SDL_WaitEvent(&event)) continue;
 
        if (event.type == SDL_QUIT) throw QuitException();
 
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_1: levels[index] = BotLevel::EASY;   return true;
                case SDLK_2: levels[index] = BotLevel::MEDIUM; return true;
                case SDLK_3: levels[index] = BotLevel::HARD;   return true;
                case SDLK_ESCAPE: return false;
                default: break;
            }
        }
    }
}

/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Đầu vào:
 *   - row: con trỏ lưu hàng.
 *   - col: con trỏ lưu cột.
 * Đầu ra:
 *   - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 *   - Cập nhật row, col nếu hợp lệ.
 */
bool SDLInteraction::getPlayerMove(int* row, int* col) {
   SDL_Event event;
 
    while (true) {
        if (!SDL_WaitEvent(&event)) continue;
 
        if (event.type == SDL_QUIT) throw QuitException();
 
        if (event.type == SDL_MOUSEBUTTONDOWN &&
            event.button.button == SDL_BUTTON_LEFT)
        {
            *row = (event.button.y - BOARD_OFFSET_Y) / CELL_SIZE;
            *col = (event.button.x - BOARD_OFFSET_X) / CELL_SIZE;
            return true;
        }
 
        if (event.type == SDL_KEYDOWN &&
            event.key.keysym.sym == SDLK_ESCAPE)
            return false;
    }
}

/**
 * Mô tả: Dọn dẹp tài nguyên liên quan đến SDL interaction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Giải phóng hoặc reset trạng thái input nếu cần.
 */
void SDLInteraction::close() {
SDL_StopTextInput();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}