/**
 * Bot LV2 cpp implementation
 *
 */

#include "bot_lv2.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel2.
 */
BotLevel2::BotLevel2(const BotLevel& _level, const char& _symbol)
    : BotLevel1(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel2.
 */
BotLevel2::~BotLevel2() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot MEDIUM.
 *
 * TODO:
 *   - Gọi heuristic để chọn nước đi
 */
pII BotLevel2::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
     return simple_heuristic(board, size, goal, symbol, op_symbol);
}
/**
 * Mô tả:
 *   Heuristic đơn giản:
 *   - Ưu tiên thắng
 *   - Chặn đối thủ
 *   - Fallback random
 *
 * TODO:
 *   - Bước 1: kiểm tra nước thắng
 *   - Bước 2: kiểm tra nước chặn
 *   - Bước 3: chọn random nếu không có
 */
pII BotLevel2::simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol) {
        int dx[4]= {0, 1, 1, -1};
    int dy[4]= {1, 0, 1, -1};
    int posti, prei;
    int postj, prej;
    for(int pos = 0 ; pos< 4 ;pos++){
        for(int i = 0 ; i< size; i++){
            for(int j = 0 ; j< size ; j++){
                int cntbot = 1;
                if(board[i][j] != botSymbol ) continue;
                int row = i + dx[pos];
                int col = j + dy[pos];
                while(0<=row && row < size && 0 <= col && col < size && board[row][col]== botSymbol){
                    cntbot++;
                    row+=dx[pos];
                    col += dy[pos];
                }
                if(cntbot == goal-1){
                    prei = i - dx[pos];
                    prej = j - dy[pos];
                    posti = i + cntbot * dx[pos];
                    postj = j + cntbot * dy[pos];
                    if(0<=posti && posti<size && 0<=postj && postj<size && board[posti][postj] == '-') return std::make_pair(posti, postj);
                    else if(0<=prei && prei<size && 0<=prej && prej<size && board[prei][prej] == '-') return std::make_pair(prei, prej);
                    else{
                        continue;
                    }
                }
            }
        }
    }
    for(int pos = 0 ; pos< 4 ;pos++){
        for(int i = 0 ; i< size; i++){
            for(int j = 0 ; j< size ; j++){
                int cntplayer =1;
            if(board[i][j] != playerSymbol ) continue;
                int row = i + dx[pos];
                int col = j + dy[pos];
                while(0<=row && row < size && 0 <= col && col < size && board[row][col]== playerSymbol){
                    cntplayer++;
                    row+=dx[pos];
                    col += dy[pos];
                }
                if(cntplayer == goal-1){
                    prei = i - dx[pos];
                    prej = j - dy[pos];
                    posti = i + cntplayer * dx[pos];
                    postj = j + cntplayer * dy[pos];
                    if(0<=posti && posti<size && 0<=postj && postj<size && board[posti][postj] == '-') return std::make_pair(posti, postj);
                    else if(0<=prei && prei<size && 0<=prej && prej<size && board[prei][prej] == '-') return std::make_pair(prei, prej);
                    else{
                        continue;
                    }
                }
            }
        }
    }
    if(board[size/2][size/2] == '-') return std:: make_pair(size/2, size/2);
    int maxcnt = 1;
    pII best = {-1, -1};
    for(int pos = 0 ; pos< 4 ;pos++){
            for(int i = 0 ; i< size; i++){
                for(int j = 0 ; j< size ; j++){
                    int cntbot = 1;
                    if(board[i][j] != botSymbol) continue;
                    int row = i + dx[pos];
                    int col = j + dy[pos];
                    while(0<=row && row < size && 0 <= col && col < size && board[row][col]== botSymbol){
                        cntbot++;
                        row+=dx[pos];
                        col += dy[pos];
                    }
                    if(cntbot > maxcnt){
                        prei = i - dx[pos];
                        prej = j - dy[pos];
                        posti = i + cntbot * dx[pos];
                        postj = j + cntbot * dy[pos];
                         if(0<=posti && posti<size && 0<=postj && postj<size && board[posti][postj] == '-') 
                         {
                            maxcnt = cntbot;
                            best = {posti, postj};
                         }
                        else if(0<=prei && prei<size && 0<=prej && prej<size && board[prei][prej] == '-'){
                            maxcnt = cntbot;
                            best = {prei, prej};
                        }
                    }
                }
            }
        }
        if(best.first != -1 && best.second != -1)return best;
        for(int i = 0 ; i< size; i++){
            for(int j = 0 ; j< size ; j++){
                if(board[i][j] != '-') continue;
                for(int pos = 0 ; pos< 4 ;pos++){
                    int row = i + dx[pos];
                    int col = j + dy[pos];
                    if(0<=row && row<size && 0<=col && col<size && board[row][col] != '-'){
                        return std:: make_pair(i, j);
                    }
                        row = i - dx[pos];
                        col = j - dy[pos];
                    if(0<=row && row<size && 0<=col && col<size && board[row][col] != '-'){
                        return std:: make_pair(i, j);
                    }
                }
            }
         }
    return random_pick(board, size);
}