#pragma once
#include "Board.h"
#include <memory>

class Player {
protected:
    char symbol;
    
public:
    Player(char sym) : symbol(sym) {}
    virtual ~Player() = default;
    virtual void getMove(Board& board) = 0;
    char getSymbol() const { return symbol; }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(char sym) : Player(sym) {}
    void getMove(Board& board) override;
};

class ComputerPlayer : public Player {
public:
    ComputerPlayer(char sym) : Player(sym) {}
    void getMove(Board& board) override;
    
private:
    bool tryWinOrBlock(Board& board, char targetSymbol);
};
