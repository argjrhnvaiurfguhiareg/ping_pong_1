#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define RACKER_SIZE 3
#define WIN_SCORE 21

/* Цветовые пары */
#define PALETTE_BORDER 1
#define PALETTE_BALL 2
#define PALETTE_P1 3
#define PALETTE_P2 4
#define PALETTE_TEXT 5

int racket_control(int res_hand, int racket_i, int racket_size, int height);
int movement_ball(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy,
                  int racket_i, int racket_j, int racket_size, int width,
                  int height);
int victory(int count_i, int count_j);
void print_victory(int ch);
void render_board(int height, int width, int ball_x, int ball_y, int racket_i,
                  int racket_j, int racket_size);

int main(void) {
  int height = HEIGHT, width = WIDTH;
  int ball_x_pos = HEIGHT / 2;
  int ball_y_pos = WIDTH / 2;
  int racket_i = HEIGHT / 2 - RACKER_SIZE / 2;
  int racket_j = HEIGHT / 2 - RACKER_SIZE / 2;
  int racket_size = RACKER_SIZE;
  int count_i = 0, count_j = 0;
  int ball_dx = 1, ball_dy = 1;
  int ch = 0;

  /* Инициализация ncurses */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  curs_set(0);
  srand(time(NULL));

  /* Настройка цветов */
  if (has_colors()) {
    start_color();
    init_pair(PALETTE_BORDER, COLOR_BLUE, COLOR_BLACK);
    init_pair(PALETTE_BALL, COLOR_RED, COLOR_BLACK);
    init_pair(PALETTE_P1, COLOR_GREEN, COLOR_BLACK);
    init_pair(PALETTE_P2, COLOR_CYAN, COLOR_BLACK);
    init_pair(PALETTE_TEXT, COLOR_WHITE, COLOR_BLACK);
  }

  while (1) {
    int input = getch();

    if (input == ' ')
      break;

    /* Управление Левым игроком (Игрок 1) */
    if (input == 'a' || input == 'A')
      racket_i = racket_control(-1, racket_i, racket_size, height);
    if (input == 'z' || input == 'Z')
      racket_i = racket_control(1, racket_i, racket_size, height);

    if (ball_dy > 0 && ball_y_pos > width / 3) {

      /* 2. Условие "Скорости": rand() % 100 < 85 делает его реакцию стабильной
         на 85%. Он не тупит на месте, а плавно и уверенно идет за мячом, но
         иногда все же ошибается */
      if (rand() % 100 < 85) {
        if (ball_x_pos > racket_j + racket_size / 2) {
          racket_j = racket_control(1, racket_j, racket_size, height);
        } else if (ball_x_pos < racket_j + racket_size / 2) {
          racket_j = racket_control(-1, racket_j, racket_size, height);
        }
      }
    } else {
      /* 3. Возврат в центр: Если мяч на чужой половине, бот не стоит как
         вкопанный, а плавно возвращается к центру поля, готовясь к следующей
         атаке. Это выглядит очень живо! */
      int center_field = height / 2 - racket_size / 2;
      if (racket_j < center_field)
        racket_j = racket_control(1, racket_j, racket_size, height);
      else if (racket_j > center_field)
        racket_j = racket_control(-1, racket_j, racket_size, height);
    }

    int event = movement_ball(&ball_x_pos, &ball_y_pos, &ball_dx, &ball_dy,
                              racket_i, racket_j, racket_size, width, height);

    if (event == 1) {
      count_j++;
      ball_x_pos = height / 2;
      ball_y_pos = width / 2;
      ball_dx = 1;
      ball_dy = 1;
    } else if (event == 2) {
      count_i++;
      ball_x_pos = height / 2;
      ball_y_pos = width / 2;
      ball_dx = -1;
      ball_dy = 1;
    }

    erase();
    render_board(height, width, ball_x_pos, ball_y_pos, racket_i, racket_j,
                 racket_size);

    /* Вывод интерфейса */
    attron(COLOR_PAIR(PALETTE_TEXT));
    mvprintw(height + 1, 2, "Player 1 (You): %d   |   Bot (AI): %d", count_i,
             count_j);
    mvprintw(height + 2, 2, "Controls: W/S to Move, Space to Quit");
    attroff(COLOR_PAIR(PALETTE_TEXT));

    refresh();

    ch = victory(count_i, count_j);
    if (ch != 0)
      break;

    usleep(40000);
  }

  endwin();

  if (ch != 0)
    print_victory(ch);
  else
    printf("Выход из игры.\n");
  return 0;
}

void render_board(int height, int width, int ball_x, int ball_y, int racket_i,
                  int racket_j, int racket_size) {
  for (int i = -1; i <= height; i++) {
    for (int j = -1; j <= width; j++) {
      /* Границы */
      if (i == -1 || i == height) {
        attron(COLOR_PAIR(PALETTE_BORDER));
        mvaddch(i + 1, j + 1, '-');
        attroff(COLOR_PAIR(PALETTE_BORDER));
      } else if (j == -1 || j == width) {
        attron(COLOR_PAIR(PALETTE_BORDER));
        mvaddch(i + 1, j + 1, '|');
        attroff(COLOR_PAIR(PALETTE_BORDER));
      }
      /* Мяч */
      else if (i == ball_x && j == ball_y) {
        attron(COLOR_PAIR(PALETTE_BALL) | A_BOLD);
        mvaddch(i + 1, j + 1, 'O');
        attroff(COLOR_PAIR(PALETTE_BALL) | A_BOLD);
      }
      /* Левая ракетка */
      else if (j == 0 && i >= racket_i && i < racket_i + racket_size) {
        attron(COLOR_PAIR(PALETTE_P1) | A_BOLD);
        mvaddch(i + 1, j + 1, '[');
        attroff(COLOR_PAIR(PALETTE_P1) | A_BOLD);
      }
      /* Правая ракетка */
      else if (j == width - 1 && i >= racket_j && i < racket_j + racket_size) {
        attron(COLOR_PAIR(PALETTE_P2) | A_BOLD);
        mvaddch(i + 1, j + 1, ']');
        attroff(COLOR_PAIR(PALETTE_P2) | A_BOLD);
      }
      /* Сетка по центру */
      else if (j == width / 2 && (i & 1)) {
        attron(COLOR_PAIR(PALETTE_BORDER));
        mvaddch(i + 1, j + 1, ':');
        attroff(COLOR_PAIR(PALETTE_BORDER));
      }
    }
  }
}

int racket_control(int res_hand, int racket_i, int racket_size, int height) {
  int res = racket_i + res_hand;
  if (res < 0)
    res = 0;
  if (res + racket_size > height)
    res = height - racket_size;
  return res;
}

int movement_ball(int *ball_x, int *ball_y, int *ball_dx, int *ball_dy,
                  int racket_i, int racket_j, int racket_size, int width,
                  int height) {
  *ball_x += *ball_dx;
  *ball_y += *ball_dy;

  if (*ball_x < 0) {
    *ball_x = 0;
    *ball_dx = -(*ball_dx);
    printf("\a");
    fflush(stdout);
  } else if (*ball_x > height - 1) {
    *ball_x = height - 1;
    *ball_dx = -(*ball_dx);
    printf("\a");
    fflush(stdout);
  }

  if (*ball_y <= 0) {
    if (*ball_x >= racket_i && *ball_x < racket_i + racket_size) {
      *ball_y = 0;
      *ball_dy = -(*ball_dy);
      printf("\a");
      fflush(stdout);
    } else {
      return 1;
    }
  }

  if (*ball_y >= width - 1) {
    if (*ball_x >= racket_j && *ball_x < racket_j + racket_size) {
      *ball_y = width - 1;
      *ball_dy = -(*ball_dy);
      printf("\a");
      fflush(stdout);
    } else {
      return 2;
    }
  }
  return 0;
}

int victory(int count_i, int count_j) {
  if (count_i >= WIN_SCORE)
    return 1;
  if (count_j >= WIN_SCORE)
    return 2;
  return 0;
}

void print_victory(int ch) {
  if (ch == 1)
    printf("\nПоздравляем! Вы победили ИИ!\n");
  else if (ch == 2)
    printf("\nПобедил ИИ. Попробуйте еще раз!\n");
}
