#include "board.h"      // Include the header file for the Board class
#include <iostream>     // For printing debug messages
#include <cstdlib>      // For rand() and srand()
#include <ctime>        // For seeding the random number generator
#include <queue>        // For using a queue in the revealCell function

#include <QDebug>       // For debugging output (optional)

// ****************************************************
// Constructor: Sets up the board with a grid of cells,
// randomly places mines, and calculates nearby mine counts.
// ****************************************************
Board::Board(int r = 16, int c = 30, int mines = 99) 
    : rows(r), cols(c), numMines(mines) {

    // Create a 2D vector (grid) with the specified number of rows and columns.
    grid.resize(rows, std::vector<Cell>(cols));

    // Seed the random number generator with the current time.
    srand(time(nullptr));

    // Place mines randomly on the board.
    placeMines();

    // For every cell, count how many mines are next to it.
    calculateAdjacentMines();
}

// ****************************************************
// Function: placeMines
// Purpose: Randomly places the given number of mines on the board.
// It keeps trying random locations until all mines are placed.
// ****************************************************
void Board::placeMines() {
    int placedMines = 0;
    // Loop until we have placed as many mines as needed.
    while (placedMines < numMines) {
        // Choose a random row and column.
        int row = rand() % rows;
        int col = rand() % cols;

        // If this cell does not already have a mine...
        if (!grid[row][col].isMine()) {
            // Place a mine in the cell.
            grid[row][col].setMine(true);
            placedMines++; // Increase the counter of placed mines.
        }
    }
}

// ****************************************************
// Function: calculateAdjacentMines
// Purpose: For each cell that is not a mine, count how many mines
// are in the 8 neighboring cells (including diagonals).
// ****************************************************
void Board::calculateAdjacentMines() {
    // Loop through every cell in the grid.
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // Skip this cell if it is a mine.
            if (grid[r][c].isMine()) 
                continue;

            int count = 0; // This will hold the number of nearby mines.

            // Look at all the neighbors (from -1 to +1 in row and column).
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr; // New row index for the neighbor.
                    int nc = c + dc; // New column index for the neighbor.

                    // Check that the neighbor is within the grid boundaries
                    // and if the neighbor is a mine, then count it.
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc].isMine()) {
                        count++;
                    }
                }
            }
            // Store the number of adjacent mines in the cell.
            grid[r][c].setAdjacentMines(count);
        }
    }
}

// ****************************************************
// Function: revealCell
// Purpose: When a player clicks on a cell, this function reveals it.
// If the revealed cell has 0 adjacent mines, it also reveals its neighbors.
// We use a simple queue (first-in, first-out) to handle revealing many cells.
// ****************************************************
void Board::revealCell(int row, int col) {
    // Check if the cell is inside the grid.
    if (row < 0 || row >= rows || col < 0 || col >= cols) 
        return;

    // Do nothing if the cell is already revealed or has a flag on it.
    if (grid[row][col].isRevealed() || grid[row][col].hasFlag()) 
        return;

    // Create a queue to keep track of which cells to reveal.
    std::queue<std::pair<int, int>> toReveal;
    // Start by adding the clicked cell.
    toReveal.push({row, col});

    // Process each cell in the queue.
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

// ****************************************************
// Function: toggleFlag
// Purpose: When a player right-clicks a cell, this function toggles
// a flag on the cell. The flag is used to mark a suspected mine.
// ****************************************************
void Board::toggleFlag(int row, int col) {
    // Make sure the cell is within the grid boundaries.
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return;

    // Only allow flagging if the cell is not already revealed.
    if (!grid[row][col].isRevealed()) {
        // Change the flag state (this might cycle through different states like flag, question mark, or no mark).
        grid[row][col].cycleMark();

        // Optionally, you can output a debug message:
        // qDebug() << "Cell" << row << col << "flag toggled.";
    }
}

// ****************************************************
// Function: isGameOver
// Purpose: Check if the game is over. The game is over if any mine
// has been revealed by the player.
// ****************************************************
bool Board::isGameOver() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // If a mine is found that has been revealed, the game is over.
            if (grid[r][c].isMine() && grid[r][c].isRevealed()) {
                return true;
            }
        }
    }
    return false;
}

// ****************************************************
// Function: isWin
// Purpose: Check if the player has won the game.
// The player wins if every cell that is not a mine is revealed.
// ****************************************************
bool Board::isWin() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            // If a safe (non-mine) cell is still hidden, the game has not been won.
            if (!grid[r][c].isMine() && !grid[r][c].isRevealed()) {
                return false;
            }
        }
    }
    return true;
}

// ****************************************************
// Function: printBoardDebug
// Purpose: For debugging, print the board to the console.
// Mines are shown as "*" and safe cells show the number of adjacent mines.
// ****************************************************
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

// ****************************************************
// Function: getGrid
// Purpose: Return the current grid of cells. This can be useful for
// other parts of the program that need to see the board state.
// ****************************************************
const std::vector<std::vector<Cell>>& Board::getGrid() const {
    return grid;
}
