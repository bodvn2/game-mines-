#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

const int SIZE = 10;
const int MINES = 10;

const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_ENTER = 13;
const int KEY_ESC = 27;

char board[SIZE][SIZE];
bool revealed[SIZE][SIZE];

int totalMines;
int remainingCells;
int remainingMines;

char prevBoard[SIZE][SIZE];
bool prevRevealed[SIZE][SIZE];
int prevRemainingCells; 
int prevRemainingMines;

void initializeBoard() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = '0';
            revealed[i][j] = false;
        }
    }
}

void placeMines() {
    srand(time(NULL));
    int count = 0;
    while (count < MINES) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;
        if (board[x][y] != '*') {
            board[x][y] = '*';
            count++;
        }
    }
}

void calculateNumbers() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] != '*') {
                int count = 0;
                for (int ni = -1; ni <= 1; ++ni) {
                    for (int nj = -1; nj <= 1; ++nj) {
                        int ni_new = i + ni;
                        int nj_new = j + nj;
                        if (ni_new >= 0 && ni_new < SIZE && nj_new >= 0 && nj_new < SIZE && board[ni_new][nj_new] == '*') {
                            count++;
                        }
                    }
                }
                board[i][j] = (count > 0) ? ('0' + count) : ' ';
            }
        }
    }
}

void printBoard() {
    system("cls");
    cout << "   ";
    for (int i = 0; i < SIZE; ++i) {
        cout << setw(3) << setfill(' ') << i + 1;
    }
    cout << endl;

    for (int i = 0; i < SIZE; ++i) {
        cout << setw(3) << setfill(' ') << i + 1;
        for (int j = 0; j < SIZE; ++j) {
            if (revealed[i][j]) {
                cout << setw(3) << setfill(' ') << board[i][j];
            } else {
                cout << setw(3) << setfill(' ') << "#";
            }
        }
        cout << endl;
    }

    cout << "Total Mines: " << totalMines << "   Remaining Mines: " << remainingMines << "   Remaining Cells: " << remainingCells << endl;
}

void copyBoardState() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            prevBoard[i][j] = board[i][j];
            prevRevealed[i][j] = revealed[i][j];
        }
    }
    prevRemainingCells = remainingCells;
    prevRemainingMines = remainingMines;
}

void restoreBoardState() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = prevBoard[i][j];
            revealed[i][j] = prevRevealed[i][j];
        }
    }
    remainingCells = prevRemainingCells;
    remainingMines = prevRemainingMines;
}

void revealCell(int row, int col) {
    if (revealed[row][col]) {
        return;
    }

    revealed[row][col] = true;
    remainingCells--;

    if (board[row][col] == ' ') {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int newRow = row + i;
                int newCol = col + j;
                if (newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE) {
                    revealCell(newRow, newCol);
                }
            }
        }
    }
}

bool isMine(int row, int col) {
    return board[row][col] == '*';
}

bool isGameOver() {
    return remainingCells == 0 || remainingMines == 0;
}

bool isGameWon() {
    return remainingCells == 0 && remainingMines == 0;
}

void resetGame() {
    initializeBoard();
    placeMines();
    calculateNumbers();
    totalMines = MINES;
    remainingCells = SIZE * SIZE - totalMines;
    remainingMines = totalMines;
}

void playGame() {
    resetGame();
    copyBoardState();

    while (true) {
        printBoard();

        if (isGameOver()) {
            if (isGameWon()) {
                cout << "Congratulations! You won!" << endl;
            } else {
                cout << "Game over! You hit a mine!" << endl;
            }

            cout << "Press P to play again, ESC to go back to the main menu." << endl;

            char input = _getch();
            if (input == 'P' || input == 'p') {
                resetGame();
                copyBoardState();
            } else if (input == 27) {
                break;
            }
        } else {
            cout << "Enter row and column (e.g., 01 02): ";
            int row, col;
            cin >> row >> col;

            if (row < 1 || row > SIZE || col < 1 || col > SIZE) {
                cout << "Invalid input. Try again." << endl;
                continue;
            }

            row--;
            col--;

            if (isMine(row, col)) {
                cout << "Game over! You hit a mine!" << endl;

                cout << "Press P to play again, ESC to go back to the main menu." << endl;

                char input = _getch();
                if (input == 'P' || input == 'p') {
                    resetGame();
                    copyBoardState();
                } else if (input == 27) {
                    break;
                }
            } else {
                revealCell(row, col);
            }
        }
    }
}

void toggleFlag(int row, int col) {
    if (!revealed[row][col]) {
        if (board[row][col] == 'F') {
            board[row][col] = '#';
            remainingMines++;
            remainingCells++;
        } else {
            board[row][col] = 'F';
            remainingMines--;
            remainingCells--;
        }
    }
}

void help() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the input buffer
    system("cls");
    cout << "===== Minesweeper - Help =====" << endl;
    cout << "Welcome to Minesweeper! Here are the rules:" << endl;
    cout << "1. The objective of the game is to uncover all the cells without hitting a mine." << endl;
    cout << "2. Each number on the board represents the number of mines in the adjacent cells." << endl;
    cout << "3. Use the 'row' and 'column' coordinates to uncover cells. For example, enter '01 02' to choose row 1, column 2." << endl;
    cout << "4. If you uncover a cell with a mine, the game is over." << endl;
    cout << "5. The game is won when all non-mine cells are uncovered, and the game is lost if you hit a mine." << endl;
    cout << "6. Press 'P' to play again or 'ESC' to return to the main menu." << endl;
    cout << "===================================" << endl;
    cout << "Press Enter to return to the main menu..." << endl;

    cin.get();  // Wait for Enter key press before returning to the main menu
}

	int main() {
    char choice;
    do {
        system("cls");
        cout << "===== Minesweeper =====" << endl;
        cout << "1. Play Game" << endl;
        cout << "2. Help" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        choice = _getch();

        switch (choice) {
            case '1':
                playGame();
                break;
            case '2':
                help();
                break;
            case '3':
                return 0;
            default:
                cout << "Invalid choice. Try again." << endl;
        }

    } while (true);

    return 0;
}
