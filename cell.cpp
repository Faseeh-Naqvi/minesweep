#include "cell.h"

Cell::Cell() : mine(false), revealed(false), flagged(false), adjacentMines(0) {}

// Setters
void Cell::setMine(bool value) { mine = value; }
void Cell::setRevealed(bool value) { revealed = value; }
void Cell::toggleFlag() { flagged = !flagged; }
void Cell::setAdjacentMines(int count) { adjacentMines = count; }

// Getters
bool Cell::isMine() const { return mine; }
bool Cell::isRevealed() const { return revealed; }
bool Cell::isFlagged() const { return flagged; }
int Cell::getAdjacentMines() const { return adjacentMines; }
