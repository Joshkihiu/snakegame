#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DELAY 100000
#define MAX_SNAKE_LEN 100
#define INITIAL_LEN 5

typedef struct {
    int x, y;
} Point;

void draw_borders(int max_x, int max_y) {
    for (int i = 0; i < max_x; i++) {
        mvprintw(0, i, "#");
        mvprintw(max_y - 1, i, "#");
    }
    for (int i = 0; i < max_y; i++) {
        mvprintw(i, 0, "#");
        mvprintw(i, max_x - 1, "#");
    }
}

void place_food(Point* food, int max_x, int max_y) {
    food->x = ((rand() % ((max_x - 4) / 2)) * 2) + 2;
    food->y = (rand() % (max_y - 2)) + 1;
}

void run_game() {
    Point snake[MAX_SNAKE_LEN];
    int snake_len = INITIAL_LEN;
    Point food;
    int max_x, max_y;
    int ch;
    int dir_x = 2, dir_y = 0;
    int game_over = 0;
    int score = 0;

    getmaxyx(stdscr, max_y, max_x);
    srand(time(NULL));

    int start_x = max_x / 2;
    int start_y = max_y / 2;
    for (int i = 0; i < snake_len; i++) {
        snake[i].x = start_x - (i * 2);
        snake[i].y = start_y;
    }

    place_food(&food, max_x, max_y);

    while (!game_over) {
        clear();
        draw_borders(max_x, max_y);

        // Draw score
        mvprintw(0, 2, " Score: %d ", score);

        // Draw food as $
        mvprintw(food.y, food.x, "$);

        // Move snake
        for (int i = snake_len - 1; i > 0; i--) {
            snake[i] = snake[i - 1];
        }

        snake[0].x += dir_x;
        snake[0].y += dir_y;

        // Collision with wall
        if (snake[0].x <= 0 || snake[0].x >= max_x - 2 ||
            snake[0].y <= 0 || snake[0].y >= max_y - 1) {
            game_over = 1;
            break;
        }

        // Self collision
        for (int i = 1; i < snake_len; i++) {
            if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
                game_over = 1;
                break;
            }
        }

        // Food collision
        if (snake[0].x == food.x && snake[0].y == food.y) {
            if (snake_len < MAX_SNAKE_LEN) {
                snake[snake_len] = snake[snake_len - 1];
                snake_len++;
                score++;
                place_food(&food, max_x, max_y);
            }
        }

        // Draw snake: head = @, body = 0
        for (int i = 0; i < snake_len; i++) {
            if (i == 0)
                mvprintw(snake[i].y, snake[i].x, "@"); // Head
            else
                mvprintw(snake[i].y, snake[i].x, "0"); // Body
        }

        refresh();
        usleep(DELAY);

        // Input
        ch = getch();
        switch (ch) {
            case KEY_UP:    if (dir_y == 0) { dir_y = -1; dir_x = 0; } break;
            case KEY_DOWN:  if (dir_y == 0) { dir_y = 1;  dir_x = 0; } break;
            case KEY_LEFT:  if (dir_x == 0) { dir_x = -2; dir_y = 0; } break;
            case KEY_RIGHT: if (dir_x == 0) { dir_x = 2;  dir_y = 0; } break;
            case 'q': game_over = 1; break;
        }
    }

    // Game over screen
    clear();
    mvprintw(max_y / 2 - 1, (max_x / 2) - 10, "Game Over! Final Score: %d", score);
    mvprintw(max_y / 2 + 1, (max_x / 2) - 15, "Press 'r' to restart or 'q' to quit.");
    refresh();

    // Wait for r or q
    nodelay(stdscr, FALSE);
    while (1) {
        ch = getch();
        if (ch == 'r') {
            run_game();
            break;
        } else if (ch == 'q') {
            break;
        }
    }
}

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    run_game();

    endwin();
    return 0;
}
