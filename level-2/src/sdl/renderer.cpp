/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"
int CELL_SIZE     = 60;
int BOARD_OFFSET_X = 100;
int BOARD_OFFSET_Y = 100;
 
static int SCREEN_WIDTH  = 800;
static int SCREEN_HEIGHT = 800;
 
/* ---------- Color Palette ---------- */
 
static constexpr SDL_Color COLOR_BG        = {20,  20,  20,  255};
static constexpr SDL_Color COLOR_GRID      = {80,  80,  80,  255};
static constexpr SDL_Color COLOR_BOARD_BG  = {40,  40,  40,  255};
static constexpr SDL_Color COLOR_X         = {220, 80,  80,  255};
static constexpr SDL_Color COLOR_O         = {80,  160, 220, 255};
static constexpr SDL_Color COLOR_HIGHLIGHT = {255, 220, 50,  120};
static constexpr SDL_Color COLOR_WINLINE   = {50,  255, 120, 160};
static constexpr SDL_Color COLOR_ERROR     = {255, 80,  80,  255};
static constexpr SDL_Color COLOR_SUCCESS   = {80,  220, 120, 255};
/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer() {
}

/**
 * Mô tả: Destructor của SDLRenderer.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không (việc giải phóng thực hiện trong close()).
 */
SDLRenderer::~SDLRenderer() {
}

/**
 * Mô tả: Khởi tạo SDL, window và renderer.
 * Đầu vào: config - cấu hình màn hình và layout.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khởi tạo SDL subsystem.
 *   - Tạo window và renderer.
 *   - Thiết lập chế độ blend.
 * NOTE: Phải gọi trước khi render.
 */
void SDLRenderer::init(const RunConfig& config) {
    // read config
    int screenWidth = config.screenWidth;
    int screenHeight = config.screenHeight;

    // int boardPadding = config.boardPadding;
    // init(...)

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "TicTacToe SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // load font
      font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 28);
    // if (!font) {
    //     // fallback: thử font hệ thống Windows
    //     font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 28);
    // }
}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen() {
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  // dark background
    SDL_RenderClear(renderer);
}

/**
 * Mô tả: Hiển thị frame đã render lên màn hình.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Swap buffer để hiển thị nội dung.
 */
void SDLRenderer::renderPresent() {
    SDL_RenderPresent(renderer);
}

/**
 * Mô tả: Vẽ hình chữ nhật lên renderer.
 * Đầu vào:
 *   - x, y: tọa độ.
 *   - w, h: kích thước.
 *   - color: màu sắc.
 *   - filled: true nếu fill, false nếu vẽ viền.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ trực tiếp lên renderer.
 */
void SDLRenderer::drawText(const std::string& text, int x, int y, SDL_Color color) {
    if (!font) return;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}
static void drawX(SDL_Renderer* ren, int x, int y, int size) {
    int margin = size / 5;
    SDL_SetRenderDrawColor(ren, COLOR_X.r, COLOR_X.g, COLOR_X.b, COLOR_X.a);
    for (int t = -1; t <= 1; t++) {
        SDL_RenderDrawLine(ren,
            x + margin + t,         y + margin,
            x + size - margin + t,  y + size - margin);
        SDL_RenderDrawLine(ren,
            x + size - margin + t,  y + margin,
            x + margin + t,         y + size - margin);
    }
}
static void drawO(SDL_Renderer* ren, int x, int y, int size) {
    int cx = x + size / 2;
    int cy = y + size / 2;
    int r  = size / 2 - size / 5;
 
    SDL_SetRenderDrawColor(ren, COLOR_O.r, COLOR_O.g, COLOR_O.b, COLOR_O.a);
 
    for (int dr = -1; dr <= 1; dr++) {
        int rr = r + dr;
        int px = rr, py = 0, err = 0;
        while (px >= py) {
            SDL_RenderDrawPoint(ren, cx + px, cy + py);
            SDL_RenderDrawPoint(ren, cx - px, cy + py);
            SDL_RenderDrawPoint(ren, cx + px, cy - py);
            SDL_RenderDrawPoint(ren, cx - px, cy - py);
            SDL_RenderDrawPoint(ren, cx + py, cy + px);
            SDL_RenderDrawPoint(ren, cx - py, cy + px);
            SDL_RenderDrawPoint(ren, cx + py, cy - px);
            SDL_RenderDrawPoint(ren, cx - py, cy - px);
            py++;
            if (err <= 0) err += 2 * py + 1;
            else { px--; err += 2 * (py - px) + 1; }
        }
    }
}
/**
 * Mô tả: Hiển thị menu lựa chọn.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ UI lên màn hình.
 * TODO:
 *   - Bước 1: Xác định loại menu.
 *   - Bước 2: Render text tương ứng.
 *   - Trường hợp biên: selectType không hợp lệ.
 */
void SDLRenderer::showSelectMenu(SelectType selectType, int context) {
    if (selectType != SelectType::PLAYER_UI) {
        clearScreen(); 
    }
    switch (selectType) {
        case SelectType::TITLE_UI:
            drawRect(0, 0, SCREEN_WIDTH, 80, {60, 120, 200, 255}, true);
            drawText("TicTacToe SDL", 300, 20, {255, 255, 255, 255});
            drawText("Press any key to start", 270, 350, {200, 200, 200, 255});
            break;

        case SelectType::SIZE_UI:
            drawRect(0, 0, SCREEN_WIDTH, 80, {60, 160, 100, 255}, true);
            drawText("Enter board size", 290, 20, {255, 255, 255, 255});
            drawRect(200, 300, 400, 80, {60, 160, 100, 180}, true);
            drawText(std::format("Range: {} - {}", BOARD_N_MIN, BOARD_N_MAX),
                     280, 320, {255, 255, 255, 255});
            drawText("Press Enter to confirm, ESC to cancel",
                     160, 420, {160, 160, 160, 255});
            break;

        case SelectType::GOAL_UI:
            drawRect(0, 0, SCREEN_WIDTH, 80, {160, 100, 60, 255}, true);
            drawText("Enter goal (win condition)", 230, 20, {255, 255, 255, 255});
            drawRect(200, 300, 400, 80, {160, 100, 60, 180}, true);
            drawText(std::format("Range: 3 - {}", std::min(context, GOAL_MAX)),
                     290, 320, {255, 255, 255, 255});
            drawText("Press Enter to confirm, ESC to cancel",
                     160, 420, {160, 160, 160, 255});
            break;

        case SelectType::GAME_MODE_UI:
            drawRect(0, 0, SCREEN_WIDTH, 80, {120, 60, 180, 255}, true);
            drawText("Select Game Mode", 280, 20, {255, 255, 255, 255});
            drawRect(150, 280, 140, 60, {80, 160, 80,  220}, true);
            drawRect(330, 280, 140, 60, {80, 80,  180, 220}, true);
            drawRect(510, 280, 140, 60, {180, 80, 80,  220}, true);
            drawText("1. PVP", 175, 300, {255, 255, 255, 255});
            drawText("2. PVE", 355, 300, {255, 255, 255, 255});
            drawText("3. EVE", 535, 300, {255, 255, 255, 255});
            break;

        case SelectType::BOT_LEVEL_UI:
        case SelectType::MUL_BOT_LEVEL_UI: {
            drawRect(0, 0, SCREEN_WIDTH, 80, {180, 60, 100, 255}, true);
            std::string title = (selectType == SelectType::MUL_BOT_LEVEL_UI)
                ? std::format("Bot {} - Select Level", context == 0 ? "X" : "O")
                : "Select Bot Level";
            drawText(title, 260, 20, {255, 255, 255, 255});
            drawRect(100, 280, 160, 60, {80,  200, 80,  220}, true);
            drawRect(320, 280, 160, 60, {200, 160, 40,  220}, true);
            drawRect(540, 280, 160, 60, {200, 60,  60,  220}, true);
            drawText("1. EASY",   120, 300, {255, 255, 255, 255});
            drawText("2. MEDIUM", 330, 300, {255, 255, 255, 255});
            drawText("3. HARD",   560, 300, {255, 255, 255, 255});
            break;
        }

        case SelectType::PLAYER_UI:
            drawRect(0, SCREEN_HEIGHT - 60, SCREEN_WIDTH, 60,
                     {60, 160, 160, 200}, true);
            drawText("Click a cell to make your move",
                     220, SCREEN_HEIGHT - 42, {255, 255, 255, 255});
            break;

        default: break;
    }

    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông báo lỗi.
 * TODO:
 *   - Bước 1: Xác định loại lỗi.
 *   - Bước 2: Render text cảnh báo.
 */
void SDLRenderer::showInvalidSelect(SelectType selectType, int context) {
    // Xóa vùng thông báo cũ
    drawRect(0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100, {20, 20, 20, 255}, true);

    std::string msg;
    switch (selectType) {
        case SelectType::SIZE_UI:
            msg = std::format("Invalid size: {}. Enter {} - {}",
                context, BOARD_N_MIN, BOARD_N_MAX);
            break;
        case SelectType::GOAL_UI:
            msg = std::format("Invalid goal: {}. Enter 3 - {}",
                context, GOAL_MAX);
            break;
        case SelectType::GAME_MODE_UI:
            msg = "Invalid mode. Press 1 = PVP, 2 = PVE, 3 = EVE";
            break;
        case SelectType::BOT_LEVEL_UI:
        case SelectType::MUL_BOT_LEVEL_UI:
            msg = "Invalid level. Press 1 = EASY, 2 = MEDIUM, 3 = HARD";
            break;
        default:
            msg = "Invalid input. Try again.";
            break;
    }

    drawRect(BOARD_OFFSET_X, SCREEN_HEIGHT - 90,
             SCREEN_WIDTH - 2 * BOARD_OFFSET_X, 50,
             {120, 30, 30, 220}, true);
    drawText(msg, BOARD_OFFSET_X + 10, SCREEN_HEIGHT - 76,
             {255, 100, 100, 255});

    renderPresent();
    SDL_Delay(500);
}
/**
 * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị xác nhận.
 * TODO:
 *   - Bước 1: Xác định loại selection.
 *   - Bước 2: Render thông báo thành công.
 */
void SDLRenderer::showValidSelect(SelectType selectType, int context) {
    // Xóa vùng thông báo
    drawRect(0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100, {20, 20, 20, 255}, true);

    std::string msg;
    switch (selectType) {
        case SelectType::SIZE_UI:
            msg = std::format("Board size: {} x {}", context, context);
            break;
        case SelectType::GOAL_UI:
            msg = std::format("Goal: {} in a row", context);
            break;
        case SelectType::GAME_MODE_UI:
            msg = std::format("Mode: {}", modeToString(context));
            break;
        case SelectType::BOT_LEVEL_UI:
        case SelectType::MUL_BOT_LEVEL_UI:
            msg = std::format("Bot level: {}", botToString(context));
            break;
        default:
            msg = "OK";
            break;
    }

    drawRect(BOARD_OFFSET_X, SCREEN_HEIGHT - 90, SCREEN_WIDTH - 2 * BOARD_OFFSET_X, 50,
             {40, 100, 60, 220}, true);
    drawText(msg, BOARD_OFFSET_X + 20, SCREEN_HEIGHT - 76, {100, 255, 150, 255});
    renderPresent();
    SDL_Delay(600);
}

/**
 * Mô tả: Vẽ bàn cờ lên màn hình.
 * Đầu vào: board, size.
 * Đầu ra: Không.
 * Tác dụng phụ: Render grid và ký hiệu.
 * TODO:
 *   - Bước 1: Tính toán layout ô.
 *   - Bước 2: Vẽ grid.
 *   - Bước 3: Vẽ X/O.
 */
void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
     int boardArea = SCREEN_WIDTH - 2 * BOARD_OFFSET_X;
    CELL_SIZE = boardArea / size;
 
    // Vẽ nền board
    drawRect(BOARD_OFFSET_X, BOARD_OFFSET_Y,
             CELL_SIZE * size, CELL_SIZE * size,
             COLOR_BOARD_BG, true);
 
    // Vẽ grid lines
    SDL_SetRenderDrawColor(renderer,
        COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, COLOR_GRID.a);
    for (int i = 0; i <= size; i++) {
        // Đường dọc
        SDL_RenderDrawLine(renderer,
            BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y,
            BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y + CELL_SIZE * size);
        // Đường ngang
        SDL_RenderDrawLine(renderer,
            BOARD_OFFSET_X,                  BOARD_OFFSET_Y + i * CELL_SIZE,
            BOARD_OFFSET_X + CELL_SIZE * size, BOARD_OFFSET_Y + i * CELL_SIZE);
    }
 
    // Vẽ X / O
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            int px = BOARD_OFFSET_X + c * CELL_SIZE + 2;
            int py = BOARD_OFFSET_Y + r * CELL_SIZE + 2;
            int cs = CELL_SIZE - 4;
 
            if      (board[r][c] == 'X') drawX(renderer, px, py, cs);
            else if (board[r][c] == 'O') drawO(renderer, px, py, cs);
        }
    }
}

/**
 * Mô tả: Hiển thị nước đi vừa thực hiện.
 * Đầu vào: row, col.
 * Đầu ra: Không.
 * Tác dụng phụ: Highlight ô.
 * TODO:
 *   - Bước 1: Xác định vị trí ô.
 *   - Bước 2: Vẽ highlight.
 */
void SDLRenderer::showMove(const int row, const int col) {
     int px = BOARD_OFFSET_X + col * CELL_SIZE;
    int py = BOARD_OFFSET_Y + row * CELL_SIZE;
    drawRect(px + 1, py + 1, CELL_SIZE - 2, CELL_SIZE - 2,
             COLOR_HIGHLIGHT, true);
    int textY = BOARD_OFFSET_Y + CELL_SIZE * BOARD_N_MAX + 10;
    drawRect(0, textY, SCREEN_WIDTH, 40, {20, 20, 20, 255}, true);
    drawText(std::format("Move placed: ({}, {})", row, col),
             BOARD_OFFSET_X, textY + 8, {180, 180, 180, 255});
}

/**
 * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị lỗi.
 * TODO:
 *   - Bước 1: Render thông báo lỗi.
 */
void SDLRenderer::showInvalidMove() {
    // Xóa vùng thông báo cũ
    drawRect(0, SCREEN_HEIGHT - 60, SCREEN_WIDTH, 60, {20, 20, 20, 255}, true);

    // Vẽ background đỏ + text
    drawRect(BOARD_OFFSET_X, SCREEN_HEIGHT - 56,
             SCREEN_WIDTH - 2 * BOARD_OFFSET_X, 50,
             {120, 30, 30, 220}, true);
    drawText("Invalid move! Please select an empty cell.",
             BOARD_OFFSET_X + 10, SCREEN_HEIGHT - 42,
             {255, 100, 100, 255});

    renderPresent();
    SDL_Delay(500);
}
/**
 * Mô tả: Hiển thị người chơi hiện tại.
 * Đầu vào: player, is_bot.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông tin turn.
 * TODO:
 *   - Bước 1: Xác định text.
 *   - Bước 2: Render lên màn hình.
 */
void SDLRenderer::showPlayer(const int player, const bool is_bot) {
     SDL_Color color = (player == 0) ? COLOR_X : COLOR_O;
 
    // Indicator vuông bên trái board
    drawRect(BOARD_OFFSET_X - 60, BOARD_OFFSET_Y, 40, 40, color, true);
 
    // Viền trắng nếu là bot
    if (is_bot)
        drawRect(BOARD_OFFSET_X - 62, BOARD_OFFSET_Y - 2, 44, 44,
                 {255, 255, 255, 180}, false);
 
    renderPresent();;
}

/**
 * Mô tả: Hiển thị kết quả game.
 * Đầu vào: winner, is_bot, winLine.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
 * TODO:
 *   - Bước 1: Kiểm tra draw hoặc win.
 *   - Bước 2: Render text.
 *   - Bước 3: Highlight winLine nếu có.
 */
void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
     // Highlight đường thắng
    if (winLine) {
        for (auto& [r, c] : winLine->cells) {
            int px = BOARD_OFFSET_X + c * CELL_SIZE;
            int py = BOARD_OFFSET_Y + r * CELL_SIZE;
            drawRect(px + 1, py + 1, CELL_SIZE - 2, CELL_SIZE - 2,
                     COLOR_WINLINE, true);
        }
    }
 
    // Banner kết quả
    int bannerY = SCREEN_HEIGHT - 80;
    if (winner == DRAW_RESULT) {
        drawRect(0, bannerY, SCREEN_WIDTH, 80, {100, 100, 100, 220}, true);
        drawRect(200, bannerY + 10, 400, 60, {150, 150, 150, 180}, true);
    } else {
        SDL_Color winColor = (winner == 0) ? COLOR_X : COLOR_O;
        winColor.a = 220;
        drawRect(0, bannerY, SCREEN_WIDTH, 80, winColor, true);
 
        // Indicator bot/human
        SDL_Color markerColor = is_bot
            ? SDL_Color{255, 200, 50,  255}
            : SDL_Color{255, 255, 255, 255};
        drawRect(340, bannerY + 12, 120, 36, markerColor, false);
    }
 
    renderPresent();
}

/**
 * Mô tả: In kết quả ra stdout (judge mode).
 * Đầu vào: gameResult.
 * Đầu ra: Không.
 * Tác dụng phụ: In console.
 * TODO:
 *   - Bước 1: Format output.
 *   - Bước 2: In ra std::cout.
 */
void SDLRenderer::printResult(const GameResult& gameResult) {
 if (gameResult.winner == DRAW_RESULT) {
        std::cout << "DRAW" << std::endl;
    } else {
        std::cout << std::format("PLAYER {} ({}) WINS in {} turns",
            gameResult.winner + 1,
            gameResult.isBot ? "BOT" : "HUMAN",
            gameResult.turns)
        << std::endl;
    }
}

/**
 * Mô tả: Giải phóng tài nguyên SDL.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Destroy renderer và window.
 *   - Shutdown SDL subsystem.
 */
void SDLRenderer::close() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}