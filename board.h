#ifndef BOARD_H
#define BOARD_H

#include <vector>    
#include "cell.h"    

// The Board class represents the Minesweeper game board.
class Board {
private:
    int rows, cols, numMines;  
    // dimensions of the board.

    
    // double vector, like a matrix
    std::vector<std::vector<Cell>> grid;

    //put mines randomly
    void placeMines();
    
    // calculates the hints
    void calculateAdjacentMines();

public:
//builds board
    Board(int r, int c, int mines);


    void revealCell(int row, int col);
    
    void toggleFlag(int row, int col);

    bool isGameOver();
    
    //Checks if the player has won the game.
    bool isWin();

    // for debugging
    // void printBoardDebug();

    const std::vector<std::vector<Cell>>& getGrid() const;
};

#endif
