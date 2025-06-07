#include "Game.h"
#include <ncurses.h>
#include <cctype>
#include <cstdlib>
#include <ctime>

void Board::drawDialogBox(int y, int x, int height, int width) const {
    attron(COLOR_PAIR(COLOR_GREEN_ON_BLACK));
    for(int j = 0; j < width; j++) {
        mvprintw(y, x+j, "-");
        mvprintw(y+height-1, x+j, "-");
    }
    for(int i = 1; i < height-1; i++) {
        mvprintw(y+i, x, "|");
        mvprintw(y+i, x+width-1, "|");
    }
    mvprintw(y, x, "+");
    mvprintw(y, x+width-1, "+");
    mvprintw(y+height-1, x, "+");
    mvprintw(y+height-1, x+width-1, "+");
    attroff(COLOR_PAIR(COLOR_GREEN_ON_BLACK));
}

void Board::display(bool showHelp) const {
    clear();
    mvprintw(1, 10, "Tic-Tac-Toe");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int color_pair = 0;
            if (grid[i][j] == 'X') color_pair = COLOR_PAIR(COLOR_YELLOW_ON_BLACK);
            if (grid[i][j] == 'O') color_pair = COLOR_PAIR(COLOR_MAGENTA_ON_BLACK);
            attron(color_pair);
            mvprintw(3 + i*2, 10 + j*4, " %c ", grid[i][j]);
            attroff(color_pair);
            if (j < 2) {
                attron(COLOR_PAIR(0));
                mvprintw(3 + i*2, 13 + j*4, "|");
                attroff(COLOR_PAIR(0));
            }
        }
        if (i < 2) {
            attron(COLOR_PAIR(0));
            mvprintw(4 + i*2, 10, "-----------");
            attroff(COLOR_PAIR(0));
        }
    }
    if(showHelp) {
        const int help_height = 8;
        const int help_width = 22;
        const int start_y = 8;
        const int start_x = 8;
        drawDialogBox(start_y, start_x, help_height, help_width);
        attron(COLOR_PAIR(COLOR_GREEN_ON_BLACK));
        mvprintw(start_y+1, start_x+2, "Key Mapping:");
        mvprintw(start_y+2, start_x+2, " W E R");
        mvprintw(start_y+3, start_x+2, " S D F");
        mvprintw(start_y+4, start_x+2, " X C V");
        mvprintw(start_y+5, start_x+2, "Press any key");
        mvprintw(start_y+6, start_x+2, "h/? for help");
        attroff(COLOR_PAIR(COLOR_GREEN_ON_BLACK));
    }
    refresh();
}

Board::Board() : grid(3, std::vector<char>(3, ' ')) {}

void Board::reset() {
    for (auto& row : grid)
        for (auto& cell : row)
            cell = ' ';
}

bool Board::isFull() const {
    for(const auto& row : grid)
        for(char c : row)
            if(c == ' ') return false;
    return true;
}

bool Board::checkWin(char symbol) const {
    for(int i = 0; i < 3; i++) {
        if(grid[i][0] == symbol && grid[i][1] == symbol && grid[i][2] == symbol) return true;
        if(grid[0][i] == symbol && grid[1][i] == symbol && grid[2][i] == symbol) return true;
    }
    if(grid[0][0] == symbol && grid[1][1] == symbol && grid[2][2] == symbol) return true;
    if(grid[0][2] == symbol && grid[1][1] == symbol && grid[2][0] == symbol) return true;
    return false;
}

bool Board::makeMove(int row, int col, char symbol) {
    if(!isValidMove(row, col)) return false;
    grid[row][col] = symbol;
    return true;
}

bool Board::isValidMove(int row, int col) const {
    return row >= 0 && row < 3 && col >= 0 && col < 3 && grid[row][col] == ' ';
}

const std::vector<std::vector<char>>& Board::getGrid() const {
    return grid;
}

void HumanPlayer::getMove(Board& board) {
    int row = -1, col = -1;
    char key;

    auto keyToPos = [](char k, int& r, int& c) -> bool {
        switch(toupper(k)) {
            case 'W': r = 0; c = 0; return true;
            case 'E': r = 0; c = 1; return true;
            case 'R': r = 0; c = 2; return true;
            case 'S': r = 1; c = 0; return true;
            case 'D': r = 1; c = 1; return true;
            case 'F': r = 1; c = 2; return true;
            case 'X': r = 2; c = 0; return true;
            case 'C': r = 2; c = 1; return true;
            case 'V': r = 2; c = 2; return true;
            default: return false;
        }
    };

    while (true) {
        board.display(false);
        mvprintw(15, 10, "Player %c - Press key for your move: ", symbol);
        refresh();
        key = getch();
        
        if (key == 'h' || key == 'H' || key == '?') {
            board.display(true);
            getch();          
            continue;             
        }

        if (!keyToPos(key, row, col)) {
            mvprintw(16, 10, "Invalid key! Use W,E,R,S,D,F,X,C,V");
            refresh();
            continue;
        }

        if (!board.isValidMove(row, col)) {
            mvprintw(16, 10, "Cell occupied! Try another key.");
            refresh();
            continue;
        }

        board.makeMove(row, col, symbol);
        move(15, 10); clrtoeol();
        move(16, 10); clrtoeol();
        break;
    }
}



bool ComputerPlayer::tryWinOrBlock(Board& board, char targetSymbol) {
    const auto& grid = board.getGrid();
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(grid[i][j] == ' ') {
                Board temp = board;
                temp.makeMove(i, j, targetSymbol);
                if(temp.checkWin(targetSymbol)) {
                    board.makeMove(i, j, symbol);
                    return true;
                }
            }
        }
    }
    return false;
}

void ComputerPlayer::getMove(Board& board) {
    mvprintw(10, 10, "Computer (%c) is thinking...", symbol);
    refresh();
    napms(500);

    if (tryWinOrBlock(board, symbol)) return;
    if (tryWinOrBlock(board, (symbol == 'X') ? 'O' : 'X')) return;

    std::vector<std::pair<int, int>> empty;
    const auto& grid = board.getGrid();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (grid[i][j] == ' ')
                empty.emplace_back(i, j);

    if (!empty.empty()) {
        auto move = empty[rand() % empty.size()];
        board.makeMove(move.first, move.second, symbol);
    }
}

Game::Game(int mode) : isPlayer1Turn(true) {
    setupPlayers(mode);
    board.reset();
}

void Game::setupPlayers(int mode) {
    player1 = std::make_unique<HumanPlayer>('X');
    if(mode == 1)
        player2 = std::make_unique<HumanPlayer>('O');
    else
        player2 = std::make_unique<ComputerPlayer>('O');
}

void Game::switchTurn() {
    isPlayer1Turn = !isPlayer1Turn;
}

bool Game::isGameOver() {
    char currentSymbol = isPlayer1Turn ? player1->getSymbol() : player2->getSymbol();

    if(board.checkWin(currentSymbol)) {
        board.display();
        mvprintw(20, 10, "Player %c wins!          ", currentSymbol);
        refresh();
        napms(1500);
        return true;
    }

    if(board.isFull()) {
        board.display();
        mvprintw(20, 10, "It's a tie!             ");
        refresh();
        napms(1500);
        return true;
    }

    return false;
}

void Game::play() {
    isPlayer1Turn = true;
    board.reset();
    
    board.display(true);
    getch(); 
    board.display(false);

    do {
        if(isPlayer1Turn)
            player1->getMove(board);
        else
            player2->getMove(board);
            
        if(isGameOver()) break;
        board.display(false);
        switchTurn();
    } while(true);
    
    board.display(false); 
}


bool Game::playAgain() const {
    mvprintw(15, 10, "Play again? (y/n): ");
    refresh();
    int choice;
    do {
        choice = getch();
    } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
    return (choice == 'y' || choice == 'Y');
}
