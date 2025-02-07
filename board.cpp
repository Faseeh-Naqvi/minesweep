#include "board.h"     
#include <cstdlib>     
#include <ctime>      
#include <queue>       
#include <QDebug>       
#include <iostream>   

Board::Board(int r = 16, int c = 30, int mines = 99) 
    : rows(r), cols(c), numMines(mines) {

    // 2D vector (grid) 
    grid.resize(rows, std::vector<Cell>(cols));

    // method for random num gen
    srand(time(nullptr));

    placeMines();

    // get the hints
    calculateAdjacentMines();
}


void Board::placeMines() {
    int placedMines = 0;
   
    while (placedMines < numMines) {
        int row = rand() % rows;
        int col = rand() % cols;

        if (!grid[row][col].isMine()) {
            grid[row][col].setMine(true);
            placedMines++;
        }
    }
}


void Board::calculateAdjacentMines() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c].isMine()) 
                continue;

            int count = 0; // This will hold the number of nearby mines.

            // Look at all the neighbors
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr;
                    int nc = c + dc; 

                    // Check that the neighbor is within the grid boundaries
                    // and if the neighbor is a mine, then count it.
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc].isMine()) {
                        count++;
                    }
                }
            }
            // Store the hint
            grid[r][c].setAdjacentMines(count);
        }
    }
}

void Board::revealCell(int row, int col) {
    // cell inside grid?
    if (row < 0 || row >= rows || col < 0 || col >= cols) 
        return;

    if (grid[row][col].isRevealed() || grid[row][col].hasFlag()) 
        return;

    // Create a queue to keep track of which cells to reveal.
    std::queue<std::pair<int, int>> toReveal;
    toReveal.push({row, col});

    // go through them
    while (!toReveal.empty()) {
        // Get the next cell's coordinates from the queue.
        auto [r, c] = toReveal.front();
        toReveal.pop();

        // Skip if this cell is already revealed or flagged.
        if (grid[r][c].isRevealed() || grid[r][c].hasFlag())
            continue;

        // Mark the cell as revealed.
        grid[r][c].setRevealed(true);

        // If this cell has no adjacent mines, add all its neighbors.
        if (grid[r][c].getAdjacentMines() == 0) {
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr;
                    int nc = c + dc;
                    // Check that the neighbor is within the grid.
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        toReveal.push({nr, nc});
                    }
                }
            }
        }
    }
}


void Board::toggleFlag(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return;

    
    if (!grid[row][col].isRevealed()) {
        // go through the flag and question markers
        grid[row][col].cycleMark();

        // qDebug() << "Cell" << row << col << "flag toggled.";//for debugging
    }
}


bool Board::isGameOver() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // If a mine is found that has been revealed, the game is over
            if (grid[r][c].isMine() && grid[r][c].isRevealed()) {
                return true;
            }
        }
    }
    return false;
}


bool Board::isWin() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!grid[r][c].isMine() && !grid[r][c].isRevealed()) {
                return false;
            }
        }
    }
    return true;
}


// void Board::printBoardDebug() {
//     for (int r = 0; r < rows; r++) {
//         for (int c = 0; c < cols; c++) {
//             if (grid[r][c].isMine()) {
//                 std::cout << "* ";
//             } else {
//                 std::cout << grid[r][c].getAdjacentMines() << " ";
//             }
//         }
//         std::cout << std::endl;
//     }
// }


const std::vector<std::vector<Cell>>& Board::getGrid() const {
    return grid;
}
