#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <vector>
#include <QGridLayout>
#include <QPushButton>

class Board;  // Forward declaration

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected: virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    
    static const int rows = 16;
    static const int cols = 30;
    static const int mines = 99;

    // UI Elements
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    std::vector<std::vector<QPushButton*>> buttonGrid;
    QIcon flagIcon;
    QIcon mineIcon;
    QIcon emptyIcon;
    QIcon questionIcon;

    // Game board pointer
    Board *gameBoard;

    // Setup and UI functions
    void setupBoard();
    void updateUI();
    void resetGame();

    // Mouse event handlers
    void handleCellClick(QPushButton *button);
    void handleRightClick(QPushButton *button);
};

#endif
