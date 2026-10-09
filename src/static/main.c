#include <stdio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define RACKER_SIZE 3
#define WIN_SCORE 21

int racket_control(int res_hand, int racket_i, int racket_size, int height);
int handling(void);
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

  printf("\x1b[2J");

  while (1) {
    /* Отрисовка кадра */
    printf("\x1b[H");
    render_board(height, width, ball_x_pos, ball_y_pos, racket_i, racket_j,
                 racket_size);
    printf("  Player 1: %d   Player 2: %d   (a/z, k/m, space = quit)\n",
           count_i, count_j);
    printf("  Ваш ход (введите символ и нажмите Enter): ");
    fflush(stdout);

    int input = handling();
    if (input == 5)
      break;

    if (input == 1)
      racket_i = racket_control(-1, racket_i, racket_size, height);
    else if (input == 2)
      racket_i = racket_control(1, racket_i, racket_size, height);
    else if (input == 3)
      racket_j = racket_control(-1, racket_j, racket_size, height);
    else if (input == 4)
      racket_j = racket_control(1, racket_j, racket_size, height);

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

    /* Проверка победы */
    ch = victory(count_i, count_j);
    if (ch != 0)
      break;
  }

  if (ch != 0)
    print_victory(ch);
  else
    printf("\nВыход.\n");
  return 0;
}

void render_board(int height, int width, int ball_x, int ball_y, int racket_i,
                  int racket_j, int racket_size) {
  for (int i = -1; i <= height; i++) {
    for (int j = -1; j <= width; j++) {
      char c = ' ';

      if (i == -1 || i == height)
        c = '-';
      else if (j == -1 || j == width)
        c = '|';
      else if (i == ball_x && j == ball_y)
        c = 'O';
      else if (j == 0 && i >= racket_i && i < racket_i + racket_size)
        c = '[';
      else if (j == width - 1 && i >= racket_j && i < racket_j + racket_size)
        c = ']';
      else if (j == width / 2 && (i & 1))
        c = ':';
      putchar(c);
    }
    putchar('\n');
  }
}

int handling(void) {
  int first_char = getchar();
  if (first_char == EOF)
    return 5;

  int rest;
  while ((rest = getchar()) != '\n' && rest != EOF)
    ;
  printf("\x1b[1A\x1b[K");

  if (first_char == 'a' || first_char == 'A')
    return 1;
  if (first_char == 'z' || first_char == 'Z')
    return 2;
  if (first_char == 'k' || first_char == 'K')
    return 3;
  if (first_char == 'm' || first_char == 'M')
    return 4;
  if (first_char == ' ')
    return 5;
  return 0;
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

  /* Верх/низ */
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

  // Проверка для левой ракетки
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

  // Проверка для правой ракетки
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
    printf("\nПобедил левый игрок!\n");
  else if (ch == 2)
    printf("\nПобедил правый игрок!\n");
}
