#ifndef GAME_H
#define GAME_H
#include <vector>
#include <memory>



#define COLOR_YELLOW_ON_BLACK 1
#define COLOR_MAGENTA_ON_BLACK 2
#define COLOR_GREEN_ON_BLACK 3


class Board {
public:
    Board();
    void display(bool showHelp = true) const;
    bool isFull() const;
    bool checkWin(char symbol) const;
    bool makeMove(int row, int col, char symbol);
    bool isValidMove(int row, int col) const;
    void reset();
    const std::vector<std::vector<char>>& getGrid() const;
private:
    void drawDialogBox(int y, int x, int height, int width) const;
    std::vector<std::vector<char>> grid;
};

class Player {
public:
    Player(char s) : symbol(s) {}
    virtual ~Player() = default;
    virtual void getMove(Board& board) = 0;
    char getSymbol() const { return symbol; }
protected:
    char symbol;
};

class HumanPlayer : public Player {
public:
    using Player::Player;
    void getMove(Board& board) override;
};

class ComputerPlayer : public Player {
public:
    using Player::Player;
    void getMove(Board& board) override;
private:
    bool tryWinOrBlock(Board& board, char targetSymbol);
};

class Game {
public:
    Game(int mode);
    void play();
    bool playAgain() const;
private:
    void switchTurn();
    bool isGameOver();
    void setupPlayers(int mode);
    Board board;
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    bool isPlayer1Turn;
};

#endif
