#include <chrono>
#include <ctime>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
const std::string VERSION = "0.4.25022997";
const bool ALGORITHM_FLAG = true;
const bool TIME_ENABLED = true;
const int RANDOM_SEED = 2013;
const int BOARD_N_MAX = 12;
const int SLEEP_TIME = 1500;
std::mt19937 generator(RANDOM_SEED);
enum class BotLevel {
    EASY,
    MEDIUM,
    HARD,
    INVALID_LV
};
enum class GameMode {
    PVP,
    PVE,
    EVE,
    INVALID_MODE
};
enum class SelectType {
    TITLE_UI,
    SIZE_UI,
    GOAL_UI,
    GAME_MODE_UI,
    BOT_LEVEL_UI,
    PLAYER_UI,
    MUL_BOT_LEVEL_UI,
    INVALID_UI
};
enum class EndRule {
    NONE,
    OPEN_ONE,
    OPEN_TWO
};
typedef std::pair<int, int> pII;
struct RunConfig {
    bool interactive = true;
    bool judge_mode = false;
    std::string input_file;

    bool to_file = true;
    std::string log_file = "log.txt";
};
struct GameSetup {
    char board[BOARD_N_MAX][BOARD_N_MAX];
    int size;
    int goal;
    GameMode mode;
    BotLevel levels[2];
};
struct GameResult {
    int winner;
    bool isBot;
    int turns;
};
constexpr int DRAW_RESULT = -1;
namespace GameLogger {
enum class Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    MSG,
};
inline std::string levelToString(Level level) {
    switch (level) {
        case Level::DEBUG:
            return "DEBUG";
        case Level::INFO:
            return "INFO";
        case Level::WARNING:
            return "WARN";
        case Level::ERROR:
            return "ERROR";
        case Level::MSG:
            return "";
        default:
            return "UNKNOWN";
    }
}
inline static Level min_level = Level::DEBUG;
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string CYAN = "\033[36m";
inline std::string getColor(Level level) {
    switch (level) {
        case Level::DEBUG:
            return BLUE;
        case Level::INFO:
            return GREEN;
        case Level::WARNING:
            return YELLOW;
        case Level::ERROR:
            return RED;
        default:
            return RESET;
    }
}
inline static std::ofstream log_file;
inline static bool write_to_file = false;
inline static bool is_judge_mode = false;
void init(bool judge_mode, bool to_file = true, const std::string& path = "log.txt");
void log(const std::string& msg, Level level = Level::INFO);
void close();
} 
RunConfig parseArgs(int argc, char* argv[]);
namespace GameInteraction {
static std::ifstream global_file_in;
}

std::streambuf* initInteraction(const RunConfig& config);
void closeInteraction(std::streambuf* cin_backup);
bool validateInput(std::string& input);
bool getInput(int* val);
bool selectSize(int* size);
bool selectGoal(int* goal, const int size);
bool selectGameMode(GameMode* mode);
bool selectBotLevel(BotLevel* levels, const int index);
bool getPlayerMove(int* row, int* col);
void clearScreen();
void showSelectMenu(SelectType selectType);
void displayBoard(const char board[][BOARD_N_MAX], const int size);
void showMove(const int row, const int col);
void showInvalidMove();
void showPlayer(const int player, const bool is_bot);
void showResult(const int winner, const bool is_bot);
void printResult(const GameResult& gameResult);
void startGame(const RunConfig& config, GameSetup& gameSetup);
GameResult playGame(const RunConfig& config, GameSetup& gameSetup);
void endGame(const RunConfig& config, GameSetup& gameSetup, GameResult& gameResult);
void initBoard(char board[][BOARD_N_MAX], const int size);
bool isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col);
void makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol);
bool isEmptyHead(char board[][BOARD_N_MAX], int size, int x, int y, const char symbol);
bool checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule = EndRule::OPEN_TWO);
bool checkDraw(char board[][BOARD_N_MAX], const int size);
pII botMove(char board[][BOARD_N_MAX], const int size, const int goal, const char symbol, const BotLevel level);
pII random_pick(char board[][BOARD_N_MAX], const int size);
pII simple_heuristic(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol);
pII hard_level(char board[][BOARD_N_MAX], const int size, const int goal, const char botSymbol, const char playerSymbol);
template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled)
    -> std::invoke_result_t<Function>;
void GameLogger::init(bool judge_mode, bool to_file, const std::string& path) {
    write_to_file = to_file;
    is_judge_mode = judge_mode;

    if (write_to_file) {
        log_file.open(path, std::ios::out | std::ios::trunc);
        if (!log_file.is_open()) {
            std::cerr << "[Logger] Cannot open log file: " << path
                      << ". Falling back to console only." << std::endl;
            write_to_file = false;  // fallback
        }
    }

    std::string header = "Tic-tac-toe Game (Version: " + std::string(VERSION) + ")\n";
    header += std::string(48, '-');

    if (write_to_file) {
        log_file << header << std::endl;
    }

    if (!is_judge_mode) {
        std::cout << header << std::endl;
    }
}

void GameLogger::log(const std::string& msg, Level level) {
    if (static_cast<int>(level) < static_cast<int>(min_level)) {
        return;
    }

    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::stringstream ss_lv;
    std::string formatted_lv;
    if (level != Level::MSG) {
        ss_lv << "[" << levelToString(level) << "]";
        formatted_lv = ss_lv.str();
    }

    std::stringstream ss_msg;
    ss_msg << (formatted_lv.empty() ? "" : " - ")
           << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] ";
    ss_msg << msg;
    std::string formatted_msg = ss_msg.str();
    if (write_to_file) {
        log_file << formatted_lv << formatted_msg << std::endl;
        log_file.flush();
    }

    if (!is_judge_mode) {
        std::cout << getColor(level) << formatted_lv;
        std::cout << getColor(Level::MSG) << formatted_msg << RESET << std::endl;
    }
}

void GameLogger::close() {
    if (log_file.is_open()) {
        log_file.close();
    }
}
RunConfig parseArgs(int argc, char* argv[]) {
    RunConfig config;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-j" || arg == "--judge") {
            config.judge_mode = true;
            config.interactive = false;

        } else if ((arg == "-i" || arg == "--input") && i + 1 < argc) {
            config.input_file = argv[++i];

        } else if ((arg == "-l" || arg == "--log") && i + 1 < argc) {
            config.log_file = argv[++i];
            if (config.log_file == "") {
                config.to_file = false;
            }

        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Tic-tac-toe Game (Version: " << VERSION << " by baluong.87)\n";
            std::cout << "Usage: \n"
                      << "\t" << "--judge, -j" << "\t" << "Enable judge mode (no UI, standard output only)\n"
                      << "\t" << "--input, -i" << "\t" << "Path to input file\n"
                      << "\t" << "--log, -l" << "\t" << "Path to log file (default: log.txt)\n";
            exit(0);
        }
    }

    return config;
}
std::streambuf* initInteraction(const RunConfig& config) {
    std::streambuf* cin_backup = nullptr;

    if (!config.interactive && !config.input_file.empty()) {
        GameInteraction::global_file_in.open(config.input_file);
        if (GameInteraction::global_file_in.is_open()) {
            cin_backup = std::cin.rdbuf();
            std::cin.rdbuf(GameInteraction::global_file_in.rdbuf());

            GameLogger::log(std::format("redirected cin to: {}", config.input_file));
        } else {
            GameLogger::log("failed to open input file, using console.", GameLogger::Level::ERROR);
        }
    }
    return cin_backup;
}
void closeInteraction(std::streambuf* cin_backup) {
    if (cin_backup) {
        std::cin.rdbuf(cin_backup);
        GameLogger::log("fallback using 'std::cin' input stream.");
    }

    if (GameInteraction::global_file_in.is_open()) {
        GameInteraction::global_file_in.close();
    }
}
bool validateInput(std::string& input) {
    if(input == "") return false;
    for(int i = 0 ; i< input.size(); i++){
        if('0'> input[i] ||'9'< input[i]) return false;
    }
    return true;
}
bool getInput(int* val) {
    std:: string input;
    std:: cin>>input;
    if(!validateInput(input)) return false;
    else{
        int data = std::stoi(input);
        *val = data;// & means address, * means value
    }
    return true;
}
bool selectSize(int* size) {
    if(!getInput(size)) return false; // size is already an adress (int*)
    else{
        if(3 >*size || *size > BOARD_N_MAX) return false;
    }
    return true;
}
bool selectGoal(int* goal, const int size) {
    if(!getInput(goal)) return false;
    if(3 <= *goal &&*goal <= size ) return true;
    return false;
}
bool selectGameMode(GameMode* mode) {
    std::string s;
    std::cin>>s;
    if(s != "1" && s!= "2" && s!= "3") {
        *mode = GameMode::INVALID_MODE;
        return false;
    }
    else{
        if(s == "1") *mode = GameMode ::PVP;
        else if(s== "2") *mode =  GameMode ::PVE;
        else if(s== "3") *mode =  GameMode :: EVE;
    }
    return true;
}
bool selectBotLevel(BotLevel* levels, const int index) {
    std::string s;
    std::cin>>s;
    if(s != "1" && s!= "2" && s!= "3") {
        levels[index] = BotLevel::INVALID_LV;
        return false;
    }
    else{
        if(s== "1") levels[index] = BotLevel :: EASY;
        else if(s=="2") levels[index] = BotLevel :: MEDIUM;
        else if(s=="3") levels[index] = BotLevel :: HARD;// level = array, *level = level[0]
    }
    return true;
}
bool getPlayerMove(int* row, int* col) {
    std ::string s,t;
    std:: cin>>s>>t;
    if(!std::cin) return false;
    
        for(int i = 0 ; i< s.size(); i++){
            if('0'> s[i] || '9'<s[i]) return false;
        }
        for(int i = 0 ; i< t.size(); i++){
            if('0'> t[i] || '9'< t[i]) return false;
        }
        *row = stoi(s); //zero
        *col = stoi(t);
    
    return true;
}
void clearScreen() {
    std::cout << "\n"
              << "\x1B[2J\x1B[H"
              << "\n";
    return;
}
void showSelectMenu(SelectType selectType) {
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;

        case SelectType::SIZE_UI:
             std::cout<< "Enter board size (NxN, 3 <= N <="<< BOARD_N_MAX <<"): ";
            break;

        case SelectType::GOAL_UI:
            std:: cout<< "Enter win goal Input (>= 3, goal <= size): ";
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
void displayBoard(const char board[][BOARD_N_MAX], const int size) {
    std::cout<<" ";
    for(int i = 0 ; i< size; i++){
        std::cout<<i<<" ";
    }
    std:: cout<<"\n";
    for(int i = 0 ; i< size; i++){
        std::cout<<"--";
    }
    std:: cout<<"\n";
    for(int i = 0 ; i<size; i++){
        std::cout<<i<<" |";
        for(int j = 0 ; j<size ; j++){
            std:: cout<<board[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}
void showPlayer(int player, bool is_bot) {
    if(is_bot){
        if(player == 0){
            std::cout<<"Bot (Player 0 (X)) is thinking..."<<"\n";
        }
        if(player == 1){
            std::cout<<"Bot (Player 1 (O)) is thinking..."<<"\n";
        }
    }
    else{
        if(player == 0){
            std:: cout<<"Player 0 (X)'s turn"<<"\n";
        }
        if(player == 1){
            std:: cout<<"Player 1 (O)'s turn"<<"\n";
        }
    }
}
void showMove(const int row, const int col) {
    std::cout<<"Move placed at ("<< row <<", "<< col<<")"<<std::endl;
}
void showInvalidMove() {
    std:: cout<<"Invalid Move!"<<std::endl;
}
void showResult(const int winner, const bool is_bot) {
    if(winner == -1) std::cout<<"It's a draw!\n";
    else if(is_bot) std::cout<<"Bot wins!\n";
    else{
        std:: cout<<"Player "<<winner<<" wins!\n";
    } 
}
void printResult(const GameResult& gameResult) {
    std::cout<<gameResult.winner<<" ";
    std::cout<<gameResult.turns<<" ";
}
void startGame(const RunConfig& config, GameSetup& gameSetup) {
    if (config.judge_mode) {
        selectSize(&gameSetup.size);
        selectGoal(&gameSetup.goal, gameSetup.size);
        selectGameMode(&gameSetup.mode);
        initBoard(gameSetup.board, gameSetup.size);
    }
    if (config.interactive) {
        clearScreen();
        showSelectMenu(SelectType::TITLE_UI);
        showSelectMenu(SelectType::SIZE_UI);
        while (!selectSize(&gameSetup.size)) {
            showSelectMenu(SelectType::SIZE_UI);
        }
        showSelectMenu(SelectType::GOAL_UI);
        while (!selectGoal(&gameSetup.goal, gameSetup.size)) {
            showSelectMenu(SelectType::GOAL_UI);
        }
        showSelectMenu(SelectType::GAME_MODE_UI);
        while (!selectGameMode(&gameSetup.mode)) {
            showSelectMenu(SelectType::GAME_MODE_UI);
        }
        if (gameSetup.mode == GameMode::PVE) {
            showSelectMenu(SelectType::BOT_LEVEL_UI);
            while (!selectBotLevel(gameSetup.levels, 1)) {
                showSelectMenu(SelectType::BOT_LEVEL_UI);
            }
        } else if (gameSetup.mode == GameMode::EVE) {
            showSelectMenu(SelectType::MUL_BOT_LEVEL_UI);
            while (!(selectBotLevel(gameSetup.levels, 0) && selectBotLevel(gameSetup.levels, 1))) {
                showSelectMenu(SelectType::MUL_BOT_LEVEL_UI);
            }
        }
        
        initBoard(gameSetup.board, gameSetup.size);
    }
}
GameResult playGame(const RunConfig& config,
                    GameSetup& gameSetup) {
    GameResult result;
    int currentPlayer = 0;
    char symbols[2] = {'X','O'};
    int turns = 0;
    int row = -1;
    int col = -1;

    while(true){
        turns++;
        if(config.interactive) displayBoard(gameSetup.board, gameSetup.size);
        bool isBot = (gameSetup.mode == GameMode::EVE) || (gameSetup.mode == GameMode::PVE && currentPlayer == 1);
        if(isBot){
            if(config.interactive) showPlayer(currentPlayer, true);
            auto point = measureExecutionTime("botMove", [&](){
                return botMove(gameSetup.board,
                               gameSetup.size,
                               gameSetup.goal,
                               symbols[currentPlayer],
                               gameSetup.levels[currentPlayer]);
            }, TIME_ENABLED);
            row = point.first;
            col = point.second;
            if(config.interactive) showMove(row, col);
        }
        else{
            if(config.interactive) showPlayer(currentPlayer, false);
            bool validInput = getPlayerMove(&row, &col);
            while(!validInput || !isValidMove(gameSetup.board, gameSetup.size, row, col)){
                showInvalidMove();
                validInput = getPlayerMove(&row, &col);
            }

            if(config.interactive) showMove(row, col);
        }
        makeMove(gameSetup.board, row, col, symbols[currentPlayer]);
        if(checkWin(gameSetup.board, gameSetup.size,
                    symbols[currentPlayer],
                    gameSetup.goal,
                    EndRule::OPEN_TWO)){
            result.winner = currentPlayer;
            result.turns = turns;
            result.isBot = (gameSetup.mode == GameMode::PVE && currentPlayer == 1);
            break;
        }
        if(checkDraw(gameSetup.board, gameSetup.size)){
            result.winner = -1;
            result.turns = turns;
            result.isBot = false;
            break;
        }

        currentPlayer = 1 - currentPlayer;
    }

    return result;
}

void endGame(const RunConfig& config,
             GameSetup& gameSetup,
             GameResult& gameResult) {
    if (config.interactive){
        clearScreen();
        displayBoard(gameSetup.board, gameSetup.size);
        showResult(gameResult.winner, gameResult.isBot);
    }
    if(config.judge_mode){
        printResult(gameResult);
    }

}
void initBoard(char board[][BOARD_N_MAX],
               const int size) {
    for(int i =0 ; i<size; i++){
        for(int j = 0 ; j<size; j++){
            board[i][j] = '-';
        }
    }
}

bool isValidMove(const char board[][BOARD_N_MAX],
                 const int size,
                 const int row,
                 const int col) {
    if(row<0 ||row >= size || col<0 || col >= size) return false;
    else if(board[row][col] != '-') return false;
    return true;
}

void makeMove(char board[][BOARD_N_MAX],
              const int row,
              const int col,
              const char symbol) {
    board[row][col] = symbol;
}

bool isEmptyHead(char board[][BOARD_N_MAX],
                    int size,
                    int x,
                    int y,
                    const char symbol) {
        if(x<0 ||x >= size || y<0 || y >= size) return true;
        else if(board[x][y] == '-') return true;
        else if(board[x][y] == symbol) return true;
        return false;
}
bool checkWin(char board[][BOARD_N_MAX],
              const int size,
              const char symbol,
              const int goal,
              EndRule rule) {
    int dx[4]= {0, 1, 1, -1};
    int dy[4]= {1, 0, 1,  1};
    for(int pos = 0 ; pos < 4; pos++){
        for(int i = 0 ; i < size; i++){
            for(int j = 0 ; j < size; j++){
                if(board[i][j] != symbol) continue;
                int cnt = 0;
                int row = i, col = j;
                while(0 <= row && row < size && 0 <= col && col < size && board[row][col] == symbol){
                    cnt++;
                    row += dx[pos];
                    col += dy[pos];
                }
                if(cnt == goal){
                    int prei = i - dx[pos], prej = j - dy[pos];
                    if(isEmptyHead(board,size,prei,prej,symbol) && isEmptyHead(board,size,row,col,symbol)) return true;
                }
            }
        }
    }
    return false;
}
bool checkDraw(char board[][BOARD_N_MAX],
               const int size) {
    for(int i = 0 ; i<size; i++){
        for(int j = 0 ; j< size; j++){
            if(board[i][j]== '-') return false;
        }
    }
    return true;
}

pII botMove(char board[][BOARD_N_MAX],
            const int size,
            const int goal,
            const char symbol,
            const BotLevel level) {
    char opponent = (symbol == 'X') ? 'O' : 'X';

    switch (level) {
        case BotLevel::EASY:
            return random_pick(board, size);

        case BotLevel::MEDIUM:
            return simple_heuristic(board, size, goal, symbol, opponent);

        case BotLevel::HARD:
            return random_pick(board, size);
        default:
            return random_pick(board, size);
    }
}
pII random_pick(char board[][BOARD_N_MAX], const int size) {
    std::vector<pII> emptycells;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == '-') {
                emptycells.push_back({i, j});
            }
        }
    }
    if (!emptycells.empty()) {
        std::uniform_int_distribution<int> dist(0, emptycells.size() - 1);
        return emptycells[dist(generator)];
    }
    return {-1, -1};
}
pII simple_heuristic(char board[][BOARD_N_MAX],
                     const int size,
                     const int goal,
                     const char botSymbol,
                     const char playerSymbol) {

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
template <typename Function>
auto measureExecutionTime(const std::string& label, Function func, bool enabled)
    -> std::invoke_result_t<Function> {
    using ReturnT = std::invoke_result_t<Function>;
    auto start = std::chrono::high_resolution_clock::now();

    if constexpr (std::is_void_v<ReturnT>) {
        func();
        auto end = std::chrono::high_resolution_clock::now();

        if (enabled) {
            std::chrono::duration<double> duration = end - start;
            std::stringstream msg;
            msg << "execution time of " << "[" << label << "()]" << " = " << duration.count() << "s";
            GameLogger::log(msg.str(), GameLogger::Level::DEBUG);
        }
        return;

    } else {
        ReturnT result = func();
        auto end = std::chrono::high_resolution_clock::now();

        if (enabled) {
            std::chrono::duration<double> duration = end - start;
            std::stringstream msg;
            msg << "execution time of " << "[" << label << "()]" << " = " << duration.count() << "s";
            GameLogger::log(msg.str(), GameLogger::Level::DEBUG);
        }

        return result;
    }
}
int main(int argc, char* argv[]) {
    RunConfig config = parseArgs(argc, argv);

    GameLogger::init(config.judge_mode, true, config.log_file);
    GameLogger::log("GameLogger initialized!");

    std::streambuf* cin_backup = initInteraction(config);
    GameLogger::log("GameInteraction initialized!");

    GameSetup gameSetup;
    startGame(config, gameSetup);
    GameLogger::log("GameEngine initialized!");

    GameResult gameResult = playGame(config, gameSetup);
    GameLogger::log("GameEngine playing done!");

    endGame(config, gameSetup, gameResult);
    GameLogger::log("GameEngine show endgame done!");

    closeInteraction(cin_backup);
    GameLogger::log("GameInteraction closed!");

    GameLogger::log("GameLogger closing . . .");
    GameLogger::close();

    return 0;
}