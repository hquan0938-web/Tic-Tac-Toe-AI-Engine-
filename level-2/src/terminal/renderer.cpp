/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {
}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer() {
    clearScreen();
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig& config) {
    this->config = config;  
    clearScreen();
    if (config.interactive) {
        std::cout << "Initializing Terminal in INTERACTIVE mode:\n";
    }
    if (config.gui_flag) {
        std::cout << "Initializing Terminal in GUI mode:\n";
    }
    if (config.verbose_flag) {
        std::cout << "Debug info: " << config.toString() << std::endl;
    }
}
/**
 * Mô tả:
 *   Xóa màn hình terminal.
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen() {
    if (!config.interactive) return;
            std::cout<< "\n"
              << "\x1B[2J\x1B[H"
              << "\n";
    return;
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    if (!config.interactive) return;
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;

        case SelectType::SIZE_UI:
             std::cout<< "Enter board size (NxN, 3 <= N <="<< BOARD_N_MAX <<"): ";
            break;

        case SelectType::GOAL_UI:
            std:: cout<< "Enter win goal Input (>= 3, goal <= 5): ";
            break;

        case SelectType::GAME_MODE_UI:
            std::cout<< "Select game mode (1) PvP | (2) PvE | (3) EvE: ";
            break;

        case SelectType::BOT_LEVEL_UI:
            std::cout<< "Select bot level (1) EASY | (2) MEDIUM | (3) HARD: ";
            break;

        case SelectType::PLAYER_UI:
            std::cout<<"Enter your move (row, col):";
            break;

        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << "Select bot levels (bot1, bot2) with modes (1) EASY | (2) MEDIUM | (3) HARD: ";
            break;

        default:
            break;
    }

    return;
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    if (!config.interactive) return;
    switch(selectType) {
        case SelectType::SIZE_UI:
            std::cout << "Invalid size! Please enter a number between 3 and " << context << ".\n";
            break;
        case SelectType::GOAL_UI:
            std::cout << "Invalid goal! Please enter a number between 3 and " << context << ".\n";
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << "Invalid game mode! Please enter 1 for PvP, 2 for PvE, or 3 for EvE.\n";
            break;
        case SelectType::BOT_LEVEL_UI:
            std::cout << "Invalid bot level! Please enter 1 for Easy, 2 for Medium, or 3 for Hard.\n";
            break;
        case SelectType::PLAYER_UI:
            std::cout << "Invalid player move! Please enter a valid move.\n";
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << "Invalid bot levels! Please enter two numbers between 1 and 3 for bot levels.\n";
            break;
        default: break;
    }                       
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
    if (!config.interactive) return;
    switch(selectType) {
        case SelectType::SIZE_UI:
            std::cout << "Board size set to " << context << "x" << context << ".\n";
            break;
        case SelectType::GOAL_UI:
            std::cout << "Win goal set to " << context << ".\n";
            break;
        case SelectType::GAME_MODE_UI:
            std::cout << "Game mode selected: ";
            switch(static_cast<GameMode>(context)) {
                case GameMode::PVP: std::cout << "PvP"; break;
                case GameMode::PVE: std::cout << "PvE"; break;
                case GameMode::EVE: std::cout << "EvE"; break;
                default:            std::cout << "Unknown"; break;
            }
            std::cout << ".\n";
            break;
        case SelectType::BOT_LEVEL_UI:
    std::cout << "Bot level set to ";
        switch(static_cast<BotLevel>(context)) {
            case BotLevel::EASY:   std::cout << "Easy";   break;
            case BotLevel::MEDIUM: std::cout << "Medium"; break;
            case BotLevel::HARD:   std::cout << "Hard";   break;
            default:               std::cout << "Unknown"; break;
        }
        std::cout << ".\n";
        break;
        case SelectType::PLAYER_UI:
            std::cout << "Player move accepted: (" << context / BOARD_N_MAX << ", " << context % BOARD_N_MAX << ").\n";
            // row = context / BOARD_N_MAX, col = context % BOARD_N_MAX
            break;
        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << "Bot levels set successfully.\n";
            break;
        default: break;
    }
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    if (!config.interactive) return;
    std::cout << "\n--- TIC TAC TOE GAME ---\n\n";
    std::cout << "    "; 
    for (int i = 0; i < size; i++) {
        std::cout << i << "   "; 
    }
    std::cout << "\n";
    std::cout << "  +";
    for (int i = 0; i < size; i++) {
        std::cout << "---+";
    }
    std::cout << "\n";
    for (int i = 0; i < size; i++) {
        // In chỉ số hàng và quân cờ
        std::cout << i << " |";
        for (int j = 0; j < size; j++) {
            char cell = board[i][j]; 
            std::cout << " " << cell << " |";
        }
        std::cout << "\n";
        std::cout << "  +";
        for (int k = 0; k < size; k++) {
            std::cout << "---+";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot) {
    if (!config.interactive) return;
    std::cout << "Player " << player << (is_bot ? " (Bot)" : "") << "'s turn.\n";
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col) {
    if (!config.interactive) return;
    std::cout << "Move played at (" << row << ", " << col << ").\n";
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove() {
    if (!config.interactive) return;
    std::cout << "Invalid move! Please try again.\n";
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    if (!config.interactive) return;
    if (winner == -1) {
        std::cout << "GAME OVER: DRAW!" << std::endl;
    } 
    else if (winner == 0) {
        std::cout << "*** PLAYER 0 WINS! ***" << std::endl;
    } 
    else if (winner == 1) {
        std::string name = is_bot ? "BOT (O)" : "PLAYER 1";
    }

    if (winner != -1 && winLine != nullptr && !winLine->cells.empty()) {
        std::cout << " Winning path: ";
        for (size_t i = 0; i < winLine->cells.size(); ++i) {
            std::cout << "(" << winLine->cells[i].first << "," << winLine->cells[i].second << ")";
            if (i < winLine->cells.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;
    }
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult& gameResult) {
    std::cout << gameResult.winner << " " << gameResult.turns << "\n";
}
/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close() {
    clearScreen();
}