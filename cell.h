#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell();

    // Setters
    void setMine(bool value);
    void setRevealed(bool value);
    void setAdjacentMines(int count);
    void cycleMark(); // cycles: 0 (no mark) -> 1 (flag) -> 2 (question) -> 0

    // Getters
    bool isMine() const;
    bool isRevealed() const;
    int  getAdjacentMines() const;

    // Mark state checkers
    bool hasFlag() const;      // returns true if markState == 1
    bool hasQuestion() const;  // returns true if markState == 2

private:
    bool mine;
    bool revealed;
    int markState;      // 0 = no mark, 1 = flag, 2 = question
    int adjacentMines;
};

#endif // CELL_H
