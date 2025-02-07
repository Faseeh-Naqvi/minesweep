#include "MainWindow.h"
#include <QVariant>
#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      rows(16),
      cols(30),
      mines(99)
{
    // Load icons
    flagIcon = QIcon(":/icons/Minesweeper_flag.svg");
    mineIcon = QIcon(":/icons/Gnome-gnomine.png");
    emptyIcon = QIcon(":/icons/Minesweeper_unopened_square.svg");

    // Setup central widget & layout
    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    setCentralWidget(centralWidget);

    // Create the Board
    gameBoard = new Board(rows, cols, mines);

    // Initialize buttons
    setupBoard();
    updateUI();
}

void MainWindow::setupBoard()
{
    // Resize our 2D vector of QPushButton pointers
    buttonGrid.resize(rows, std::vector<QPushButton*>(cols));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = new QPushButton();
            button->setFixedSize(30, 30);

            // Add to layout
            gridLayout->addWidget(button, r, c);
            buttonGrid[r][c] = button;

            // Store row & col as a dynamic property
            button->setProperty("row", r);
            button->setProperty("col", c);

            // **Important**: Install event filter to detect mouse events (right-click)
            button->installEventFilter(this);
        }
    }
}

/**
 * Event Filter to detect right-click (and left-click).
 * Called before the QPushButton sees the event.
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPushButton *button = qobject_cast<QPushButton*>(obj);

        if (button) {
            if (mouseEvent->button() == Qt::RightButton) {
                // Right-click => handle flag toggling
                handleRightClick(button);
                return true; // Stop further processing
            } else if (mouseEvent->button() == Qt::LeftButton) {
                // Left-click => reveal cell
                handleCellClick(button);
                return true; // Stop further processing
            }
        }
    }
    // For other events, default behavior
    return QMainWindow::eventFilter(obj, event);
}

/**
 * handleCellClick -> reveals the cell if left-clicked
 */
void MainWindow::handleCellClick(QPushButton *button)
{
    if (!button) return;

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    gameBoard->revealCell(row, col);
    updateUI();

    if (gameBoard->isGameOver()) {
        QMessageBox::StandardButton result = QMessageBox::critical(
            this,
            "Uh Oh!",
            "You have hit a mine and exploded!",
            QMessageBox::Retry | QMessageBox::Close
        );
        if (result == QMessageBox::Retry) {
            resetGame();
        } else {
            close();
        }
    }
    else if (gameBoard->isWin()) {
        QMessageBox::information(this, "Congratulations!", "You won!", QMessageBox::Ok);
    }
}

/**
 * handleRightClick -> toggles flag if right-clicked
 */
void MainWindow::handleRightClick(QPushButton *button)
{
    if (!button) return;

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    gameBoard->toggleFlag(row, col);
    updateUI();
}

/**
 * Resets the board to a new game
 */
void MainWindow::resetGame()
{
    // Discard old board
    delete gameBoard;
    gameBoard = new Board(rows, cols, mines);

    // Clear out old button states
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

/**
 * Refresh all QPushButtons according to the underlying Board state
 */
void MainWindow::updateUI()
{
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QPushButton *button = buttonGrid[r][c];
            Cell cell = gameBoard->getGrid()[r][c];

            if (cell.isFlagged()) {
                button->setIcon(flagIcon);
            } else if (cell.isRevealed()) {
                if (cell.isMine()) {
                    button->setIcon(mineIcon);
                    button->setStyleSheet("background-color: red;");
                } else {
                    button->setIcon(emptyIcon);
                    button->setText(QString::number(cell.getAdjacentMines()));
                    button->setStyleSheet("background-color: lightgray;");
                }
            } else {
                // Hidden cell: no icon or text
                button->setIcon(QIcon());
                button->setText("");
            }
        }
    }
}

MainWindow::~MainWindow()
{
    // Cleanup
    delete gameBoard;
}
