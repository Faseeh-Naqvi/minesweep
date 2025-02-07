#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell();

    // Set cell properties
    void setMine(bool value);
    void setRevealed(bool value);
    void setAdjacentMines(int count);
    void cycleMark(); // cycles: 0 -> 1 -> 2 -> 0

    // Get cell properties
    bool isMine() const;
    bool isRevealed() const;
    int getAdjacentMines() const;

    // Check mark state
    bool hasFlag() const;     // true if markState == 1
    bool hasQuestion() const; // true if markState == 2

private:
    bool mine;        // true if the cell is a mine
    bool revealed;    // true if the cell is revealed
    int markState;    // 0: none, 1: flag, 2: question
    int adjacentMines;// number of mines around the cell
};

#endif // CELL_H
