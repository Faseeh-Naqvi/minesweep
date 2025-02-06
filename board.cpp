#include "board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>

Board::Board(int r = 16, int c = 30, int mines = 99) : rows(r), cols(c), numMines(mines) {
    grid.resize(rows, std::vector<Cell>(cols));

    srand(time(nullptr)); // Seed for randomness
    placeMines();
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
            if (grid[r][c].isMine()) continue;

            int count = 0;
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc].isMine()) {
                        count++;
                    }
                }
            }
            grid[r][c].setAdjacentMines(count);
        }
    }
}

void Board::revealCell(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) return;
    if (grid[row][col].isRevealed() || grid[row][col].isFlagged()) return;

    std::queue<std::pair<int, int>> toReveal;
    toReveal.push({row, col});

    while (!toReveal.empty()) {
        auto [r, c] = toReveal.front();
        toReveal.pop();

        if (grid[r][c].isRevealed() || grid[r][c].isFlagged()) continue;

        grid[r][c].setRevealed(true);

        // If no adjacent mines, add its neighbors to the queue
        if (grid[r][c].getAdjacentMines() == 0) {
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                        toReveal.push({nr, nc});
                    }
                }
            }
        }
    }
}

void Board::toggleFlag(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) return;
    if (!grid[row][col].isRevealed()) {
        grid[row][col].toggleFlag();
    }
}

bool Board::isGameOver() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c].isMine() && grid[r][c].isRevealed()) {
                return true; // A mine was revealed!
            }
        }
    }
    return false;
}

bool Board::isWin() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!grid[r][c].isMine() && !grid[r][c].isRevealed()) {
                return false; // Not all safe cells are revealed
            }
        }
    }
    return true; // All non-mine cells are revealed
}

void Board::printBoardDebug() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c].isMine()) {
                std::cout << "* ";
            } else {
                std::cout << grid[r][c].getAdjacentMines() << " ";
            }
        }
        std::cout << std::endl;
    }
}

const std::vector<std::vector<Cell>>& Board::getGrid() const {
    return grid;
}
