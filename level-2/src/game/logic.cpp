/**
 * Logic cpp implementation
 *
 */
#include<iostream>
#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {

    for(int i =0 ; i<size; i++){
        for(int j = 0 ; j<size; j++){
            board[i][j] = '-';
        }
    }
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    if(row<0 ||row >= size || col<0 || col >= size) return false;
    else if(board[row][col] != '-') return false;
    return true;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    board[row][col] = symbol;
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
   if(x<0 ||x >= size || y<0 || y >= size) return true;
    Logger::log(std::format("[DEBUG] isEmptyHead checking ({},{})", x, y), Logger::Level::DEBUG);
    if(board[x][y] == '-') return true;
    if(board[x][y] == symbol) return true;
    return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    return getWinLine(board, size, symbol, goal, rule).has_value();
}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    for(int i = 0 ; i< size; i++){
        for(int j = 0 ; j< size; j++){
            if(board[i][j] == '-') return false;
        }
    }
    return true;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {
    int dx[4] = {0, 1, 1, -1};
    int dy[4] = {1, 0, 1, -1};

    for (int pos = 0; pos < 4; pos++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] != symbol) continue;

                int pi = i - dx[pos], pj = j - dy[pos];
                if (pi >= 0 && pi < size && pj >= 0 && pj < size
                    && board[pi][pj] == symbol) continue;

                std::vector<std::pair<int,int>> cells;
                int row = i, col = j;
                while (0 <= row && row < size && 0 <= col && col < size
                       && board[row][col] == symbol) {
                    cells.push_back({row, col});
                    row += dx[pos];
                    col += dy[pos];
                }

                if ((int)cells.size() >= goal) {
                    int prei = i - dx[pos], prej = j - dy[pos];
                    if (isEmptyHead(board, size, prei, prej, symbol) &&
                        isEmptyHead(board, size, row,  col,  symbol))
                        return WinLine{cells};
                }
            }
        }
    }

    return std::nullopt;
}