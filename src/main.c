#include <stdio.h>


#define WIDTH 80
#define HEIGHT 25
#define RACKER_SIZE 3

void render_board(int height, int width, int ball_x, int ball_y, int racket_i, int racket_j, int racket_size);
void moving_ball(int ball_x, int ball_y);
int racket_control(int res_hand, int racket_i, int racket_size, int height);
int handling(); 
//int movement_ball(int ball_x, int ball_y, int ball_dx, int ball_dy, int racket_i, int racket_j, int count_i, int count_j, int racket_size, int width, int height);



int main(void) {
    int height = HEIGHT, width = WIDTH, ball_x_pos = WIDTH/2, ball_y_pos = HEIGHT/2, racket_i = 5, racket_j=10, racket_size=RACKER_SIZE;
    //int count_i = 0, count_j = 0;
    //int ball_dx = 1, ball_dy = 1;
    render_board(height, width, ball_x_pos, ball_y_pos, racket_i, racket_j, racket_size);
    while (1) {
        int input = handling();
        if (input == 1) racket_i = racket_control(-1, racket_i, racket_size, width);
        else if (input == 2) racket_i = racket_control(1, racket_i, racket_size, width);
        else if (input == 3) racket_j = racket_control(-1, racket_j, racket_size, width);
        else if (input == 4) racket_j = racket_control(1, racket_j, racket_size, width);

        render_board(height, width, ball_x_pos, ball_y_pos, racket_i, racket_j, racket_size);
    }

    return 0;
}



void render_board(int height, int width, int ball_x, int ball_y, int racket_i, int racket_j, int racket_size) {
    for (int i = -1; i <= height; i++) {
        for (int j = -1; j <= width; j ++) {
            char c = ' ';
            if (j==0 && i >= racket_i && i < racket_i + racket_size) c = '[';
            else if (j==width-1 && i >= racket_j && i < racket_j + racket_size) c = ']';
            else if (i == -1 || i == height) c = '-';
            else if (j == -1 || j == width) c = '|';
            else if (j == (width)/2) c = '|';
            else if (i == ball_x && j == ball_y) c = 'O';
            printf("%c", c);
        }
        printf("\n");
    }
}



int handling() {
    int res = 0;
    while (res == 0) {
        char c  = getchar();
        //while (getchar() != '\n' || getchar() != EOF);
        if (c == 'a' || c =='A') res = 1;
        else if (c == 'z' || c == 'Z') res = 2;
        else if (c == 'k' || c == 'K') res = 3;
        else if (c == 'm' || c == 'M') res = 4;
        else if (c == ' ') res = 5;
    }
    return res;
}

int racket_control(int res_hand, int racket_i, int racket_size, int height) {
    int res = racket_i + res_hand;
    if (res < 0) res = 0;
    else if (res + racket_size > height) res = height - racket_size;
    return res;
}

//int movement_ball(int ball_x, int ball_y, int ball_dx, int ball_dy, int racket_i, int racket_j, int count_i, int count_j, int racket_size, int width, int height) {
    //int target_x = ball_x + ball_dx, target_y = ball_y + ball_dy;
    //if (target_x == -1 || target_x == height) {
        //ball_dx *= -1;
        //target_x = ball_x + ball_dx;
    //}

    
//}








