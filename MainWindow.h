#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include "board.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // Override eventFilter to catch right-click
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void setupBoard();
    void updateUI();
    void resetGame();
    
    // Handle left-click and right-click with pointer to the button clicked
    void handleCellClick(QPushButton *button);
    void handleRightClick(QPushButton *button);

    // Members
    QIcon flagIcon, mineIcon, emptyIcon, questionIcon;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    Board *gameBoard;
    std::vector<std::vector<QPushButton*>> buttonGrid;
    int rows, cols, mines;
};

#endif // MAINWINDOW_H
