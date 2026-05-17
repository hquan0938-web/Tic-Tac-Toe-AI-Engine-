/**
 * Bot LV3 cpp implementation
 *
 */

#include "bot_lv3.h"

/* ---------- Importing ---------- */

#include <algorithm>
#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"

/* ---------- Definitions ---------- */

BotLevel3::BotLevel3(const BotLevel& _level, const char& _symbol)
    : BotLevel2(_level, _symbol) {
}

BotLevel3::~BotLevel3() {
}

// ─── getMaxDepth ─────────────────────────────────────────────────────────────

static int getMaxDepth(const int size) {
    if (size <= 3) return 9;
    if (size <= 5) return 5;
    if (size <= 7) return 4;
    return 3;
}

// ─── evaluate ────────────────────────────────────────────────────────────────

static int evaluate(
    char board[][BOARD_N_MAX],
    const int size,
    const int goal,
    const char botSymbol,
    const char playerSymbol)
{
    if (Logic::checkWin(board, size, botSymbol,    goal)) return  SCORE_INF;
    if (Logic::checkWin(board, size, playerSymbol, goal)) return -SCORE_INF;
    if (Logic::checkDraw(board, size))                    return  0;

    int score = 0;
    int dx[4] = {0, 1, 1, -1};
    int dy[4] = {1, 0, 1, -1};

    for (int pos = 0; pos < 4; pos++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int cntbot = 0, playercnt = 0;
                int row = i, col = j;
                bool valid = true;

                for (int k = 0; k < goal; k++) {
                    if (row < 0 || row >= size || col < 0 || col >= size) {
                        valid = false;
                        break;
                    }
                    if      (board[row][col] == botSymbol)    cntbot++;
                    else if (board[row][col] == playerSymbol) playercnt++;
                    row += dx[pos];
                    col += dy[pos];
                }

                if (!valid) continue;
                if (cntbot > 0 && playercnt == 0)
                    score += EVALUATE_SCORE * cntbot;
                if (playercnt > 0 && cntbot == 0)
                    score -= EVALUATE_SCORE * playercnt;
            }
        }
    }

    return score;
}

// ─── alphaBeta ───────────────────────────────────────────────────────────────

static int alphaBeta(
    char board[][BOARD_N_MAX],
    const int size,
    const int goal,
    const char botSymbol,
    const char playerSymbol,
    int depth,
    int alpha,
    int beta,
    bool ismax)
{
    int eval = evaluate(board, size, goal, botSymbol, playerSymbol);
    if (depth == 0 || eval == SCORE_INF || eval == -SCORE_INF ||
        Logic::checkDraw(board, size))
        return eval;

    if (ismax) {
        int best = -SCORE_INF;
        bool cut = false;
        for (int i = 0; i < size && !cut; i++) {
            for (int j = 0; j < size && !cut; j++) {
                if (board[i][j] != '-') continue;

                board[i][j] = botSymbol;
                int score = alphaBeta(board, size, goal,
                    botSymbol, playerSymbol,
                    depth - 1, alpha, beta, false);
                board[i][j] = '-';

                best  = std::max(best, score);
                alpha = std::max(alpha, best);
                if (beta <= alpha) cut = true;
            }
        }
        return best;

    } else {
        int best = SCORE_INF;
        bool cut = false;
        for (int i = 0; i < size && !cut; i++) {
            for (int j = 0; j < size && !cut; j++) {
                if (board[i][j] != '-') continue;

                board[i][j] = playerSymbol;
                int score = alphaBeta(board, size, goal,
                    botSymbol, playerSymbol,
                    depth - 1, alpha, beta, true);
                board[i][j] = '-';

                best = std::min(best, score);
                beta = std::min(beta, best);
                if (beta <= alpha) cut = true;
            }
        }
        return best;
    }
}

// ─── hard_level ──────────────────────────────────────────────────────────────

pII BotLevel3::hard_level(char board[][BOARD_N_MAX], const int size, const int goal) {
    int depth = getMaxDepth(size);
    int best  = -SCORE_INF - 1;
    pII bestMove = {-1, -1};

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] != '-') continue;

            board[i][j] = symbol;
            int score = alphaBeta(board, size, goal,
                symbol, op_symbol,
                depth - 1, -SCORE_INF, SCORE_INF, false);
            board[i][j] = '-';

            if (score > best) {
                best     = score;
                bestMove = {i, j};
            }
        }
    }

    return bestMove;
}

// ─── getMove ─────────────────────────────────────────────────────────────────

pII BotLevel3::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    pII move = hard_level(board, size, goal);
    if (move.first != -1 && move.second != -1) return move;
    return BotLevel2::getMove(board, size, goal);
}