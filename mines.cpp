#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>

// константы для уровней сложности
const int EASY_SIZE = 9;
const int EASY_MINES = 1;
const int MEDIUM_SIZE = 16;
const int MEDIUM_MINES = 40;
const int HARD_SIZE = 24;
const int HARD_MINES = 99;

// класс для игры "Сапер"
class Minesweeper {
private:
    int size; // размер поля
    int mines; // количество мин
    int flags; // количество флагов
    int openedCells; // количество открытых ячеек
    std::vector<std::vector<char>> board; // игровое поле
    std::vector<std::vector<bool>> mineMap; // карта мин
    bool gameOver; // клаг окончания игры

    // генерация мин после первого хода
    void generateMines(int firstX, int firstY) {
        srand(time(0));
        int placedMines = 0;
        while (placedMines < mines) {
            int x = rand() % size;
            int y = rand() % size;
            if (!mineMap[x][y] && (x != firstX || y != firstY)) {
                mineMap[x][y] = true;
                placedMines++;
            }
        }
    }

    // подсчет мин вокруг ячейки
    int countMinesAround(int x, int y) {
        int count = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int nx = x + i;
                int ny = y + j;
                if (nx >= 0 && nx < size && ny >= 0 && ny < size && mineMap[nx][ny]) {
                    count++;
                }
            }
        }
        return count;
    }

    // открытие ячейки
    void openCell(int x, int y) {
        if (x < 0 || x >= size || y < 0 || y >= size || board[x][y] != '#') return;

        if (mineMap[x][y]) {
            gameOver = true;
            return;
        }

        int minesAround = countMinesAround(x, y);
        board[x][y] = (minesAround == 0) ? ' ' : ('0' + minesAround);
        openedCells++;

        if (minesAround == 0) {
            // рекурсия для открытия соседних ячеек
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    openCell(x + i, y + j);
                }
            }
        }
    }

public:
    Minesweeper(int size, int mines) : size(size), mines(mines), flags(0), openedCells(0), gameOver(false) {
        // инициализация поля
        board = std::vector<std::vector<char>>(size, std::vector<char>(size, '#'));
        mineMap = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
    }

    // вывод поля
    void printBoard() {
        std::cout << "  ";
        for (int i = 0; i < size; ++i) {
            std::cout << i << " ";
        }
        std::cout << "\n";

        for (int i = 0; i < size; ++i) {
            std::cout << i << " ";
            for (int j = 0; j < size; ++j) {
                std::cout << board[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    // обработка хода игрока
    void makeMove() {
        int x, y;
        char action;
        std::cout << "Введите координаты (x y) и действие (o - открыть, f - флаг): ";
        std::cin >> x >> y >> action;

        if (x < 0 || x >= size || y < 0 || y >= size) {
            std::cout << "Неверные координаты!\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return;
        }

        if (action == 'o') {
            if (board[x][y] == '#') {
                if (openedCells == 0) {
                    generateMines(x, y); // генерация мин после первого хода
                }
                openCell(x, y);
            } else {
                std::cout << "Эта ячейка уже открыта!\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        } else if (action == 'f') {
            if (board[x][y] == '#') {
                board[x][y] = 'F';
                flags++;
            } else if (board[x][y] == 'F') {
                board[x][y] = '#';
                flags--;
            } else {
                std::cout << "Нельзя поставить флаг на открытую ячейку!\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        } else {
            std::cout << "Неверное действие!\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    // проверка условия победы
    bool checkWin() {
        return openedCells == size * size - mines;
    }

    // основной цикл
    void play() {
        std::cout << "Добро пожаловать в игру 'Сапер'!\n";
        while (!gameOver) {
            printBoard();
            makeMove();

            if (gameOver) {
                system("clear");

                std::cout << "  ########     ######      ######    ######     #####  ##  ##" << std::endl;
                std::cout << "  ##    ##    ##    ##    ##    ##   ##  ##    ##  ##  ##  ##" << std::endl;
                std::cout << "  ##   ##    ##      ##  ##      ##  ##   ##  ##   ##  ##  ##" << std::endl;
                std::cout << "  #######    ##      ##  ##      ##  ##    ####    ##  ##  ##" << std::endl;
                std::cout << "  ##   ##    ##      ##  ##      ##  ##            ##  ##  ##" << std::endl;
                std::cout << "  ##    ##   ##      ##  ##      ##  ##            ##  ##  ##" << std::endl;
                std::cout << "  ##    ##    ##    ##    ##    ##   ##            ##        " << std::endl;
                std::cout << "  ########     ######      ######    ######    ######  ##  ##" << std::endl;

                std::this_thread::sleep_for(std::chrono::seconds(1));

                system("clear");

                std::cout << "Вы проиграли! Вы наткнулись на мину.\n";
                break;
            }

            if (checkWin()) {
                std::cout << "Поздравляем! Вы выиграли!\n";
                break;
            }

            system("clear");

        }

        std::cout << "Игра окончена. Всего мин: " << mines << ", найдено мин: " << flags << "\n";
    }

    void showLogo() {

        std::cout << "Добро пожаловать в игру 'Сапёр'!\n";
        std::cout << "Начало через: \n";
        for (int i = 3; i > 0; i--) {
            std::cout << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        system("clear");

    }

};

int main() {
    int size, mines;

    std::cout << "Выберите уровень сложности:\n";
    std::cout << "1 - Легкий (9x9, 10 мин)\n";
    std::cout << "2 - Средний (16x16, 40 мин)\n";
    std::cout << "3 - Сложный (24x24, 99 мин)\n";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            size = EASY_SIZE;
            mines = EASY_MINES;
            break;
        case 2:
            size = MEDIUM_SIZE;
            mines = MEDIUM_MINES;
            break;
        case 3:
            size = HARD_SIZE;
            mines = HARD_MINES;
            break;
        default:
            std::cout << "Неверный выбор. Выбран легкий уровень.\n";
            size = EASY_SIZE;
            mines = EASY_MINES;
            break;
    }

    Minesweeper game(size, mines);

    game.showLogo();
    game.play();

    return 0;
}