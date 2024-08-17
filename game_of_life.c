#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define ROWS 26
#define COLS 81

void output_static(int matrix[ROWS][COLS], int rows, int cols);
void change_condition(int matrix[ROWS][COLS], int matrix_copy[ROWS][COLS]);
int check_around(int matrix_check[ROWS][COLS], int index_i, int index_j);
void get_copy(int matrix_from[ROWS][COLS], int matrix_to[ROWS][COLS]);
void fill_borders(int matrix[ROWS][COLS]);
char convert_symbol(int num);
int input(int a[ROWS][COLS], int rows, int cols);
void clear_terminal();
int life_count(int matrix[ROWS][COLS], int rows, int cols);
int change_time(char factor);
int birth_death(int matrix_copy[ROWS][COLS], int i, int j);

int main() {
    clear_terminal();
    initscr();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);
    noecho();
    int matrix[ROWS][COLS] = {0};
    int err_flag = input(matrix, ROWS, COLS);
    int generation = 0;
    output_static(matrix, ROWS, COLS);
    int life_counter = 0;
    refresh();
    if (freopen("/dev/tty", "r", stdin) != NULL) {
        int speed = 100000;
        while (!err_flag) {
            char k = getch();
            if (k >= 49 && k <= 53) {
                speed = change_time(k);
            }
            clear_terminal();
            int tmpmatrix[ROWS][COLS] = {0};
            get_copy(matrix, tmpmatrix);
            change_condition(matrix, tmpmatrix);
            output_static(matrix, ROWS, COLS);
            life_counter = life_count(matrix, ROWS, COLS);
            printw("Generation: %d\n", generation);
            printw("Life count: %d\n", life_counter);
            if (life_counter == 0) {
                err_flag = 1;
            }
            generation++;
            usleep(speed);
            if (k == 'q' || k == 'Q') {
                err_flag = 1;
            }
            refresh();
        }
    }

    endwin();
    printw("Life is over\n");
    printw("Generation: %d\n", generation);
    printw("Life count: %d\n", life_counter);
    return 0;
}

int life_count(int matrix[ROWS][COLS], int rows, int cols) {
    int count = 0;
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            count += matrix[i][j];
        }
    }
    return count;
}

int input(int a[ROWS][COLS], int rows, int cols) {
    int err_flag = 0;
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            if (scanf("%d", &a[i][j]) != 1) {
                err_flag = 1;
                break;
            }
        }
        if (err_flag) {
            break;
        }
    }
    fill_borders(a);
    return err_flag;
}

void clear_terminal() {
    clear();
    return;
}

void fill_borders(int matrix[ROWS][COLS]) {
    for (int i = 0; i < COLS; i++) {
        matrix[0][i] -= 1;
        matrix[ROWS - 1][i] -= 1;
    }
    for (int j = 0; j < ROWS; j++) {
        matrix[j][0] -= 2;
        matrix[j][COLS - 1] -= 2;
    }
    return;
}

char convert_symbol(int num) {
    char symbol = (char)0;
    if (num == 1) {
        symbol = '*';
    } else if (num == -2) {
        symbol = '|';
    } else if (num == -1) {
        symbol = '-';
    } else if (num == -3) {
        symbol = '#';
    } else if (num == 0) {
        symbol = ' ';
    }
    return symbol;
}

void output_static(int matrix[ROWS][COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j != cols - 1) {
                printw("%c", convert_symbol(matrix[i][j]));
            } else {
                printw("%c", convert_symbol(matrix[i][j]));
            }
        }
        if (i != rows - 1) {
            printw("\n");
        }
    }
    printw("\n");
    return;
}

void get_copy(int matrix_from[ROWS][COLS], int matrix_to[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            matrix_to[i][j] = matrix_from[i][j];
        }
    }
    return;
}

void change_condition(int matrix[ROWS][COLS], int matrix_copy[ROWS][COLS]) {
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLS - 1; j++) {
            matrix[i][j] = birth_death(matrix_copy, i, j);
        }
    }
    return;
}

int birth_death(int matrix_copy[ROWS][COLS], int i, int j) {
    int dec = 0;
    if (matrix_copy[i][j]) {
        if (check_around(matrix_copy, i, j) == 3 || check_around(matrix_copy, i, j) == 2) {
            dec = 1;
        } else {
            dec = 0;
        }
    } else {
        if (check_around(matrix_copy, i, j) == 3) {
            dec = 1;
        }
    }
    return dec;
}

int check_around(int matrix_check[ROWS][COLS], int index_i, int index_j) {
    int res = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (!i && !j) {
                res += 0;
            } else if (!(index_i + i) && !(index_j + j)) {
                res += matrix_check[ROWS - 2][COLS - 2];
            } else if (!(index_i + i) && (index_j + j) == COLS - 1) {
                res += matrix_check[ROWS - 2][1];
            } else if (!(index_j + j) && (index_i + i) == ROWS - 1) {
                res += matrix_check[1][COLS - 2];
            } else if (!(index_i + i)) {
                res += matrix_check[ROWS - 2][index_j + j];
            } else if (!(index_j + j)) {
                res += matrix_check[index_i + i][COLS - 2];
            } else if (index_i + i == ROWS - 1 && index_j + j == COLS - 1) {
                res += matrix_check[1][1];
            } else if (index_i + i == ROWS - 1) {
                res += matrix_check[1][index_j + j];
            } else if (index_j + j == COLS - 1) {
                res += matrix_check[index_i + i][1];
            } else {
                res += matrix_check[index_i + i][index_j + j];
            }
        }
    }
    return res;
}

int change_time(char factor) {
    int speed = 100000;
    if (factor == 49) {
        speed = 500000;
    } else if (factor == 50) {
        speed = 250000;
    } else if (factor == 51) {
        speed = 125000;
    } else if (factor == 52) {
        speed = 50000;
    } else {
        speed = 20000;
    }
    return speed;
}
