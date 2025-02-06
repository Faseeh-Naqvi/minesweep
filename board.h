#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "cell.h"

class Board {
private:
    int rows, cols, numMines;
    std::vector<std::vector<Cell>> grid;

    void placeMines(); // Places mines randomly
    void calculateAdjacentMines(); // Calculates adjacent mine counts

public:
    Board(int r, int c, int mines);
    
    void revealCell(int row, int col);
    void toggleFlag(int row, int col);
    bool isGameOver();
    bool isWin();
    
    void printBoardDebug(); // Debugging (optional)
    
    const std::vector<std::vector<Cell>>& getGrid() const; // Getter for GUI

};

#endif // BOARD_H
