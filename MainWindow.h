#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QIcon>
#include "board.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
QIcon flagIcon, mineIcon, emptyIcon;    
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    Board *gameBoard;
    std::vector<std::vector<QPushButton*>> buttonGrid;
    int rows, cols, mines;

    void setupBoard();
    void resetGame();
    void updateUI();

private slots:
    void handleCellClick();
    void handleRightClick();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
