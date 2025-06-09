#pragma once
#include <vector>
#include <iostream>

class Board {
private:
    std::vector<std::vector<char>> grid;
    
public:
    Board();
    void display() const;
    bool isFull() const;
    bool checkWin(char symbol) const;
    bool makeMove(int row, int col, char symbol);
    bool isValidMove(int row, int col) const;
    const std::vector<std::vector<char>>& getGrid() const;
};
