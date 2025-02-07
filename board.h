#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "cell.h"

class Board {
private:
    int rows, cols, numMines;
    std::vector<std::vector<Cell>> grid;

    void placeMines();
    void calculateAdjacentMines();

public:
    Board(int r, int c, int mines);

    void revealCell(int row, int col);
    void toggleFlag(int row, int col);

    bool isGameOver();
    bool isWin();

    void printBoardDebug();

    const std::vector<std::vector<Cell>>& getGrid() const;
};

#endif // BOARD_H
