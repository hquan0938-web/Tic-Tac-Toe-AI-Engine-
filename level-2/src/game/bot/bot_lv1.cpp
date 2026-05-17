/**
 * Bot LV1 cpp implementation
 *
 */

#include "bot_lv1.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel1.
 *
 * Đầu vào:
 *   - _level: mức độ bot
 *   - _symbol: ký hiệu bot
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Gọi constructor của lớp cha Bot
 */
BotLevel1::BotLevel1(const BotLevel& _level, const char& _symbol)
    : Bot(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel1.
 *
 * Đầu vào:
 *   - Không có
 *
 * Đầu ra:
 *   - Không có
 *
 * Tác dụng phụ:
 *   - Không có
 */
BotLevel1::~BotLevel1() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot level EASY.
 *
 * Đầu vào:
 *   - board, size, goal
 *
 * Đầu ra:
 *   - pII: nước đi (row, col)
 *
 * Tác dụng phụ:
 *   - Không có
 *
 * TODO:
 *   - Sử dụng chiến lược đơn giản (random)
 */
pII BotLevel1::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    return random_pick(board, size);
}

/**
 * Mô tả:
 *   Chọn ngẫu nhiên một ô trống trên bàn cờ.
 *
 * Đầu vào:
 *   - board, size
 *
 * Đầu ra:
 *   - pII: ô được chọn
 *
 * Tác dụng phụ:
 *   - Có thể log debug
 *
 * TODO:
 *   - Bước 1: Duyệt toàn bộ board để thu thập các ô trống
 *   - Bước 2: Nếu không có ô → return (-1, -1)
 *   - Bước 3: Random chọn một ô trong danh sách
 *   - Trường hợp biên: board đầy
 */
pII BotLevel1::random_pick(char board[][BOARD_N_MAX], const int size) {
    std::vector<pII> emptycells;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '-') {
                emptycells.push_back({i, j});
            }
        }
    }
    if (!emptycells.empty()) {
        int range = (int)emptycells.size() - 1;
        if (range < 0) range = 0;  // Safety check
        std::uniform_int_distribution<int> dist(0, range);
        return emptycells[dist(generator)];
    }
    return {-1, -1};
}