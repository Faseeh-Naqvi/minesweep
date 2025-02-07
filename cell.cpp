#include "cell.h"

Cell::Cell() 
    : mine(false),
      revealed(false),
      markState(0),  // no mark
      adjacentMines(0)
{}

void Cell::setMine(bool value) { mine = value; }
void Cell::setRevealed(bool value) { revealed = value; }
void Cell::setAdjacentMines(int count) { adjacentMines = count; }

bool Cell::isMine() const { return mine; }
bool Cell::isRevealed() const { return revealed; }
int Cell::getAdjacentMines() const { return adjacentMines; }

void Cell::cycleMark() {
    // Cycle marks: 0 -> 1 -> 2 -> 0
    markState = (markState + 1) % 3;
}

bool Cell::hasFlag() const { return markState == 1; }
bool Cell::hasQuestion() const { return markState == 2; }
