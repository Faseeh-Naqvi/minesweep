#include "MainWindow.h"
#include "board.h"       
#include <QVariant>
#include <QApplication>
#include <QMouseEvent>
#include <QGridLayout>
#include <QMessageBox>
#include <QGridLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Load the icons for the game pieces
    flagIcon     = QIcon(":/icons/Minesweeper_flag.svg");
    mineIcon     = QIcon(":/icons/Gnome-gnomine.png");
    emptyIcon    = QIcon(":/icons/Minesweeper_unopened_square.svg");
    questionIcon = QIcon(":/icons/Minesweeper_questionmark.svg");

    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    setCentralWidget(centralWidget);

    gameBoard = new Board(rows, cols, mines);

    // Create the buttons grid and update the display
    setupBoard();
    updateUI();
}

void MainWindow::setupBoard()
{
    buttonGrid.resize(rows, std::vector<QPushButton*>(cols));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = new QPushButton();
            button->setFixedSize(30, 30);
            gridLayout->addWidget(button, r, c);
            buttonGrid[r][c] = button;
            // Save row and column numbers in the button
            button->setProperty("row", r);
            button->setProperty("col", c);
            // Let MainWindow handle the mouse clicks
            button->installEventFilter(this);
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Check if the event is a mouse press
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPushButton *button = qobject_cast<QPushButton*>(obj);
        if (button) {
            if (mouseEvent->button() == Qt::RightButton) {
                handleRightClick(button);
                return true;
            } else if (mouseEvent->button() == Qt::LeftButton) {
                handleCellClick(button);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::handleCellClick(QPushButton *button)
{
    if (!button) return;
    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    gameBoard->revealCell(row, col);
    updateUI();

    // Check if the player hit a mine or won the game
    if (gameBoard->isGameOver()) {
        auto result = QMessageBox::critical(this, "Uh Oh!", "You hit a mine!",
                        QMessageBox::Retry | QMessageBox::Close);
        if (result == QMessageBox::Retry)
            resetGame();
        else
            close();
    }
    else if (gameBoard->isWin()) {
        QMessageBox::information(this, "Congratulations!", "You won!", QMessageBox::Ok);
    }
}

void MainWindow::handleRightClick(QPushButton *button)
{
    if (!button) return;
    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    gameBoard->toggleFlag(row, col);
    updateUI();
}

void MainWindow::resetGame()
{
    delete gameBoard;
    gameBoard = new Board(rows, cols, mines);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = buttonGrid[r][c];
            button->setIcon(QIcon());
            button->setText("");
            button->setStyleSheet("");
        }
    }
    updateUI();
}

void MainWindow::updateUI()
{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            QPushButton *button = buttonGrid[r][c];
            const Cell &cell = gameBoard->getGrid()[r][c];

            if (cell.hasFlag()) {
                button->setIcon(flagIcon);
                button->setText("");
                button->setStyleSheet("");
            } else if (cell.hasQuestion()) {
                button->setIcon(questionIcon);
                button->setText("");
                button->setStyleSheet("");
            } else if (cell.isRevealed()) {
                if (cell.isMine()) {
                    button->setIcon(mineIcon);
                    button->setText("");
                    button->setStyleSheet("background-color: red;");
                } else {
                    button->setIcon(emptyIcon);
                    button->setText(cell.getAdjacentMines() > 0 ? QString::number(cell.getAdjacentMines()) : "");
                    button->setStyleSheet("background-color: lightgray;");
                }
            } else {
                button->setIcon(QIcon());
                button->setText("");
                button->setStyleSheet("");
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete gameBoard;
}
