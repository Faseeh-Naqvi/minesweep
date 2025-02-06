
#ifndef CELL_H
#define CELL_H

class Cell {
private:
    bool mine;
    bool revealed;
    bool flagged;
    int adjacentMines;

public:
    Cell();

    // Setters
    void setMine(bool value);
    void setRevealed(bool value);
    void toggleFlag();
    void setAdjacentMines(int count);

    // Getters
    bool isMine() const;
    bool isRevealed() const;
    bool isFlagged() const;
    int getAdjacentMines() const;
};

#endif // CELL_H
