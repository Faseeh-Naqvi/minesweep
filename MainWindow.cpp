#include "MainWindow.h"
#include <QVariant>

#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      rows(16), 
      cols(30), 
      mines(99)
{
    // Initialize icons (no "private:" here!)
    flagIcon = QIcon(":/icons/Minesweeper_flag.svg");
    mineIcon = QIcon(":/icons/Gnome-gnomine.png");
    emptyIcon = QIcon(":/icons/Minesweeper_unopened_square.svg");

    centralWidget = new QWidget(this);
    gridLayout = new QGridLayout();
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    gameBoard = new Board(rows, cols, mines);
    setupBoard();
}



void MainWindow::setupBoard() {
    buttonGrid.resize(rows, std::vector<QPushButton*>(cols));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            QPushButton *button = new QPushButton();
            button->setFixedSize(30, 30);

            buttonGrid[r][c] = button;
            gridLayout->addWidget(button, r, c);

            // Store row & col inside the button
            button->setProperty("row", r);
            button->setProperty("col", c);

            // Connect to "pressed", not "clicked"
            connect(button, &QPushButton::pressed, this, [this]() {
                // Check if it’s a right-click or left-click
                if (QApplication::mouseButtons() & Qt::RightButton) {
                    handleRightClick();
                } else {
                    handleCellClick();
                }
            });
        }
    }
}


void MainWindow::handleCellClick() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    gameBoard->revealCell(row, col);
    updateUI();

    if (gameBoard->isGameOver()) {
        // This opens a critical dialog with "Retry" and "Close"
        QMessageBox::StandardButton result = QMessageBox::critical(
            this, 
            "Uh Oh!", 
            "You have hit a mine and exploded!", 
            QMessageBox::Retry | QMessageBox::Close
        );

        if (result == QMessageBox::Retry) {
            resetGame(); // You must implement this
        } else {
            close(); // or qApp->quit();
        }
    } else if (gameBoard->isWin()) {
        QMessageBox::information(this, "Congratulations!", "You won!", QMessageBox::Ok);
    }
}

void MainWindow::resetGame() {
    // Delete the old board
    delete gameBoard;
    
    // Create a fresh Board
    gameBoard = new Board(rows, cols, mines);
    
    // If you want to preserve the *same* buttons, 
    // just clear any old state or icons:
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            QPushButton* button = buttonGrid[r][c];
            button->setIcon(QIcon());
            button->setText("");
            button->setStyleSheet("");
        }
    }
    
//update display
    updateUI();
}





void MainWindow::handleRightClick() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    gameBoard->toggleFlag(row, col);
    updateUI();
}


void MainWindow::updateUI() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
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
                button->setIcon(QIcon());
                button->setText("");
            }
        }
    }
}



MainWindow::~MainWindow() {
    delete gameBoard;
}
