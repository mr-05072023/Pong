#include <math.h>
#include <stdio.h>

// A - ДВИЖЕНИЕ РАКЕТКИ ВВЕРХ(ЛЕВЫЙ)
// Z - ДВИЖЕНИЕ РАКЕТКИ ВНИЗ(ЛЕВЫЙ)
// K - ДВИЖЕНИЕ РАКЕТКИ ВВЕРХ(ПРАВЫЙ)
// M - ДВИЖЕНИЕ РАКЕТКИ ВНИЗ(ПРАВЫЙ)

// РАЗМЕРЫ ПОЛЯ И РАКЕТОК
#define HEIGHT 25
#define WIDTH 80
#define RACKET_WIDTH 3

// ВЕКТОР(НАПРАВЛЕНИЕ) ДВИЖЕНИЯ РАКЕТОК
#define LEFT_TOP 3
#define LEFT_BOT 4
#define RIGHT_TOP 5
#define RIGHT_BOT 6

// ВЕКТОР(НАПРАВЛЕНИЕ) ДВИЖЕНИЯ МЯЧА
#define _MIDDLE 7
#define MIDDLE 8
#define _TOP_LEFT 9
#define TOP_RIGHT 10
#define _BOT_LEFT 11
#define BOT_RIGHT 12

// ПОКАЗАТЕЛЬ ТОГО, С ЧЕМ ИМЕННО СТОЛКНУЛСЯ МЯЧ
#define TOP 13
#define BOT 14
#define LEFT 15
#define RIGHT 16
#define LEFT_RACKET 17
#define RIGHT_RACKET 18

// СЧЕТ
int SCORE_FIRST = 0;
int SCORE_SECOND = 0;

// НАЧАЛЬНАЯ ПОЗИЦИЯ МЯЧА
int x_ball = WIDTH / 2;
int y_ball = HEIGHT / 2;

//  НАЧАЛЬНАЯ ПОЗИЦИЯ РАКЕТОК
int racket_point_left = HEIGHT / 2 - RACKET_WIDTH / 2;
int racket_point_right = HEIGHT / 2 - RACKET_WIDTH / 2;

void interface();
void interface_def();
void print_racket_and_ball_def(int x, int y, int x_ball, int y_ball, int racket_point_left,
                               int racket_point_right);
// int recalc_vec_board(int input, int vec_board);
int recalc_x_ball(int *vec_ball, int x_ball);
int recalc_y_ball(int *vec_ball, int y_ball);
int triger(int x_ball, int y_ball, int *vec_ball, int racket_point_left, int racket_point_right);
int reload();

int main() {
    // ОБЪЯВЛЕНИЕ ДЛЯ ВЕКТОРОВ РАКЕТОК
    // int racket_vec_left, racket_vec_right;
    int vec_ball = BOT_RIGHT;

    // ПЕЧАТАЕМ ПОЛЕ(НАЧАЛЬНАЯ ПОЗИЦИЯ)
    interface_def();

    // ПРИНИМАЕМ ВВОД СИМВОЛА
    do {
        char input;
        input = getchar();
        if (input == 'a') {
            if (!(racket_point_left < 2)) {
                racket_point_left -= 2;
            }
        } else if (input == 'z') {
            if (!(racket_point_left > HEIGHT - 5)) {
                racket_point_left += 2;
            }
        } else if (input == 'k') {
            if (!(racket_point_right < 2)) {
                racket_point_right -= 2;
            }
        } else if (input == 'm') {
            if (!(racket_point_right > HEIGHT - 5)) {
                racket_point_right += 2;
            }
        }

        // СТОЛКНЕТСЯ ЛИ С ЧЕМ-ТО МЯЧ
        int trigger = triger(x_ball, y_ball, &vec_ball, racket_point_left, racket_point_right);
        if (trigger == 1) {
            if (SCORE_FIRST > SCORE_SECOND) printf("Player 1 win. Congratulations!");
            if (SCORE_SECOND > SCORE_FIRST) printf("Player 2 win. Congratulations!");
            return 0;
        } else if (trigger == 2) {
            x_ball = WIDTH / 2;
            y_ball = HEIGHT / 2;
            vec_ball = _TOP_LEFT;
            racket_point_left = HEIGHT / 2 - RACKET_WIDTH / 2;
            racket_point_right = HEIGHT / 2 - RACKET_WIDTH / 2;
            interface_def();
            continue;
        }
        // ИЗМЕНЕНИЕ КООРДИНАТ МЯЧА ОТНОСИТЕЛЬНО ВЕКТОРА
        x_ball = recalc_x_ball(&vec_ball, x_ball);
        y_ball = recalc_y_ball(&vec_ball, y_ball);
        interface_def();
    } while (SCORE_FIRST != 21 && SCORE_SECOND != 21);
    return 0;
}

void interface_def() {
    // ОЧИСТА ТЕРМИНАЛА
    printf("\e[1;1H\e[2J");
    // ГОРИЗОНТАЛЬНАЯ
    printf("+");
    for (int i = 0; i < (WIDTH - 2); i++) printf("-");
    printf("+\n");
    // left line vertical
    for (int y = 0; y < HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < (WIDTH - 2); x++) {
            print_racket_and_ball_def(x, y, x_ball, y_ball, racket_point_left, racket_point_right);
        }
        // ПРАВАЯ ВЕРТИКАЛЬНАЯ
        printf("|\n");
    }

    // ГОРИЗОНТАЛЬНАЯ
    printf("+");
    for (int i = 0; i < (WIDTH - 2); i++) printf("-");
    printf("+\n");
    printf("%20d%40d\n\n", SCORE_FIRST, SCORE_SECOND);
}

void print_racket_and_ball_def(int x, int y, int x_ball, int y_ball, int racket_point_left,
                               int racket_point_right) {
    if (x == x_ball && y == y_ball)
        printf("*");
    else if (y >= racket_point_left && y < (racket_point_left + RACKET_WIDTH) && x == 5)
        printf("|");
    else if (y >= racket_point_right && y < (racket_point_right + RACKET_WIDTH) && x == WIDTH - 8)
        printf("|");
    else
        printf(" ");
}

int recalc_x_ball(int *vec_ball, int x_ball) {
    // ИЗМЕНЕНИЕ КООРДИНАТЫ МЯЧА \ X \ В ЗАВИСИМОСТИ ОТ ВЕКТОРА
    if (*vec_ball == _MIDDLE || *vec_ball == _TOP_LEFT || *vec_ball == _BOT_LEFT) {
        x_ball -= 2;
    } else {
        x_ball += 2;
    }
    return x_ball;
}

int recalc_y_ball(int *vec_ball, int y_ball) {
    // ИЗМЕНЕНИЕ КООРДИНАТЫ МЯЧА \ Y \ В ЗАВИСИМОСТИ ОТ ВЕКТОРА
    if (*vec_ball == _MIDDLE || *vec_ball == MIDDLE) {
        return y_ball;
    } else if (*vec_ball == _BOT_LEFT || *vec_ball == BOT_RIGHT) {
        y_ball += 1;
    } else if (*vec_ball == _TOP_LEFT || *vec_ball == TOP_RIGHT) {
        y_ball -= 1;
    }
    return y_ball;
}

int triger(int x_ball, int y_ball, int *vec_ball, int racket_point_left, int racket_point_right) {
    // СТОЛКНЕТСЯ ЛИ С ЧЕМ-ТО МЯЧ
    int triger;
    if ((x_ball == 72) && (y_ball == racket_point_right || y_ball == racket_point_right + 1 ||
                           y_ball == racket_point_right - 1 || y_ball == racket_point_right + 2 ||
                           y_ball == racket_point_right - 2)) {
        triger = 0;
        if (*vec_ball == MIDDLE) {
            *vec_ball = _MIDDLE;
        } else if (*vec_ball == TOP_RIGHT) {
            *vec_ball = _TOP_LEFT;
        } else {
            *vec_ball = _BOT_LEFT;
        }
    } else if ((x_ball == 6) && (y_ball == racket_point_left || y_ball == racket_point_left + 1 ||
                                 y_ball == racket_point_left - 1 || y_ball == racket_point_left + 2 ||
                                 y_ball == racket_point_left - 2)) {
        triger = 0;
        if (*vec_ball == _MIDDLE) {
            *vec_ball = MIDDLE;
        } else if (*vec_ball == _TOP_LEFT) {
            *vec_ball = TOP_RIGHT;
        } else {
            *vec_ball = BOT_RIGHT;
        }
    } else if (y_ball == 0) {
        triger = 0;
        if (*vec_ball == _TOP_LEFT) {
            *vec_ball = _BOT_LEFT;
        } else {
            *vec_ball = BOT_RIGHT;
        }
    } else if (y_ball == 24) {
        triger = 0;
        if (*vec_ball == BOT_RIGHT) {
            *vec_ball = TOP_RIGHT;
        } else if (*vec_ball == _BOT_LEFT) {
            *vec_ball = _TOP_LEFT;
        }
    } else if (x_ball == 0) {
        // ОБНОВЛЕНИЕ СЧЕТА И ПЕРЕЗАПУСК
        triger = 0;
        SCORE_SECOND += 1;
        if (reload() == 1) {
            triger = 1;
        } else {
            triger = 2;
        }
    } else if (x_ball == 76) {
        // ОБНОВЛЕНИЕ СЧЕТА И ПЕРЕЗАПУСК
        triger = 0;
        SCORE_FIRST += 1;
        if (reload() == 1) {
            triger = 1;
        } else {
            triger = 2;
        }
    }
    return triger;
}

int reload() {
    if (SCORE_FIRST == 21 || SCORE_SECOND == 21) {
        interface_def();
        if (SCORE_FIRST > SCORE_SECOND) printf("\t\t\tPlayer 1 won. Congratulations!\n");
        if (SCORE_SECOND > SCORE_FIRST) printf("\t\t\tPlayer 2 won. Congratulations!\n");
        SCORE_FIRST = 0;
        SCORE_SECOND = 0;
        return 1;
    } else {
        return 0;
    }
}