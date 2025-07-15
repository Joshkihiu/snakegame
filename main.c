#include <ncurses.h> // Essential for making our terminal game come alive
#include <stdlib.h>  // Needed for random numbers
#include <time.h>    // Used with srand() to get different random numbers each time
#include <unistd.h>  // Not strictly used now

// Let's set up some game rules and dimensions
#define BOARD_WIDTH 60
#define BOARD_HEIGHT 30
#define INITIAL_SNAKE_LENGTH 5
#define GAME_SPEED_MS 150 // How fast the snake moves (lower number means faster game!)

// A simple way to track positions on our board
typedef struct {
    int x;
    int y;
} Point;

// These variables keep track of everything happening in our game
Point snake[BOARD_WIDTH * BOARD_HEIGHT]; // Our snake's body segments (fixed size for safety)
int snake_length; // Current length of our snake
Point food; // Where the tasty food is
int score; // How many points we've racked up
int direction; // Which way the snake is currently heading (0: Up, 1: Down, 2: Left, 3: Right)
int game_over; // A flag to know if the game has ended
int best_score = 0; // Keep track of the highest score achieved

// Let's declare the functions we'll be using
void setup_game();
void draw_board();
void generate_food();
void handle_input();
void update_game_state();
void game_loop();
int display_game_over();

int main() {
    // First, we get ncurses ready to take over our terminal
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    // Initial timeout is now handled inside the loop for each game session

    // Let's make sure our food appears in different places each time we play
    srand(time(NULL));

    int play_again = 1; // Start by wanting to play!

    // This loop keeps the game going as long as the player wants to play again
    while (play_again) {
        setup_game();
        // Ensure the game loop's non-blocking timeout is set for each new game
        timeout(GAME_SPEED_MS);
        game_loop();
        play_again = display_game_over(); // After the game ends, ask if they want another go
    }

    // When the player is done, we clean up ncurses
    endwin();

    // And finally, show the score one last time in the regular terminal
    printf("Thanks for playing! Your final score: %d\n", score);
    printf("Best Score: %d\n", best_score); // Display best score on final exit

    return 0;
}

// This function sets up all the initial values for a new game
void setup_game() {
    snake_length = INITIAL_SNAKE_LENGTH;
    score = 0;
    game_over = 0;
    direction = 3; // Let's start by moving right

    // Place the snake's head roughly in the middle of the board
    snake[0].x = BOARD_WIDTH / 2;
    snake[0].y = BOARD_HEIGHT / 2;

    // And then position the rest of the snake's body segments behind the head
    for (int i = 1; i < snake_length; i++) {
        snake[i].x = snake[0].x - i;
        snake[i].y = snake[0].y;
    }

    generate_food(); // Time to place the first delicious food item!
}

// This function is responsible for drawing everything you see on the game screen
void draw_board() {
    clear(); // Clear the entire screen first

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Calculate where to start drawing to center our game board
    // We add +2 for the borders, and +4 for the score, best score, and quit messages below the board
    int start_x = (cols - (BOARD_WIDTH + 2)) / 2;
    int start_y = (rows - (BOARD_HEIGHT + 2 + 4)) / 2; // Adjusted for 4 lines below

    // Make sure our starting points aren't negative (if the terminal is too small)
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;

    // Draw the top and bottom walls of our game area
    for (int i = 0; i < BOARD_WIDTH + 2; i++) {
        mvprintw(start_y + 0, start_x + i, "#");
        mvprintw(start_y + BOARD_HEIGHT + 1, start_x + i, "#");
    }
    // Draw the left and right walls
    for (int i = 0; i < BOARD_HEIGHT + 2; i++) {
        mvprintw(start_y + i, start_x + 0, "#");
        mvprintw(start_y + i, start_x + BOARD_WIDTH + 1, "#");
    }

    // Explicitly clear the playable area within the borders
    // This is a robust way to remove any lingering characters (like the '0' glitch)
    for (int y = 1; y <= BOARD_HEIGHT; y++) {
        mvprintw(start_y + y, start_x + 1, "%*s", BOARD_WIDTH, ""); // Print spaces across the row
    }

    // Draw the snake! The head is '@', and the body segments are 'o'
    for (int i = 0; i < snake_length; i++) {
        mvprintw(start_y + snake[i].y + 1, start_x + snake[i].x + 1, (i == 0) ? "@" : "o");
    }

    // Draw the food that the snake needs to eat
    mvprintw(start_y + food.y + 1, start_x + food.x + 1, "*");

    // Show the current score and best score on separate lines
    mvprintw(start_y + BOARD_HEIGHT + 2, start_x + 1, "Score: %d", score);
    mvprintw(start_y + BOARD_HEIGHT + 3, start_x + 1, "Best Score: %d", best_score); // Display best score on a new line
    mvprintw(start_y + BOARD_HEIGHT + 4, start_x + 1, "Press 'q' to quit."); // Shift quit message down

    refresh(); // Update the terminal screen to show everything we've drawn
}

// This function finds a new spot for the food, making sure it doesn't appear on the snake
void generate_food() {
    int on_snake;
    do {
        on_snake = 0;
        food.x = rand() % BOARD_WIDTH;
        food.y = rand() % BOARD_HEIGHT;

        // Check if the new food position is on top of any part of the snake
        for (int i = 0; i < snake_length; i++) {
            if (food.x == snake[i].x && food.y == snake[i].y) {
                on_snake = 1; // Oh no, it's on the snake! Try again.
                break;
            }
        }
    } while (on_snake); // Keep trying until we find a clear spot
}

// This function listens for what the player is typing
void handle_input() {
    int ch = getch();

    switch (ch) {
        case KEY_UP:
            if (direction != 1) direction = 0; // Prevent immediate reverse
            break;
        case KEY_DOWN:
            if (direction != 0) direction = 1; // Prevent immediate reverse
            break;
        case KEY_LEFT:
            if (direction != 3) direction = 2; // Prevent immediate reverse
            break;
        case KEY_RIGHT:
            if (direction != 2) direction = 3; // Prevent immediate reverse
            break;
        case 'q':
            game_over = 1; // Time to end the game
            break;
    }
}

// This is where all the game logic happens: moving the snake, checking for trouble, and eating
void update_game_state() {
    // First, let's move the snake's body. Each segment just follows the one in front of it.
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Now, move the snake's head based on the current direction
    switch (direction) {
        case 0: snake[0].y--; break; // Up (decrease Y)
        case 1: snake[0].y++; break; // Down (increase Y)
        case 2: snake[0].x--; break; // Left (decrease X)
        case 3: snake[0].x++; break; // Right (increase X)
    }

    // Check if the snake hit a wall
    if (snake[0].x < 0 || snake[0].x >= BOARD_WIDTH ||
        snake[0].y < 0 || snake[0].y >= BOARD_HEIGHT) {
        game_over = 1; // Game over, you hit a wall!
        return;
    }

    // Check if the snake bit itself (ouch!)
    for (int i = 1; i < snake_length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            game_over = 1; // Game over, self-collision!
            return;
        }
    }

    // Did the snake eat the food?
    if (snake[0].x == food.x && snake[0].y == food.y) {
        score++; // Yay! One point for eating the fruit!
        // Make the snake grow longer. We just increase the length, and the next update
        // will naturally add a new segment at the previous tail's spot.
        if (snake_length < BOARD_WIDTH * BOARD_HEIGHT) { // Don't let it grow too big!
            snake_length++;
        }
        generate_food(); // Place new food for the next meal
    }
}

// This is the heart of our game, keeping everything moving and checking for updates
void game_loop() {
    while (!game_over) { // Keep playing as long as the game isn't over
        handle_input();
        update_game_state();
        draw_board();
    }
}

// This function is called when the game ends, and it asks the player if they want to restart
int display_game_over() {
    clear();

    // Update best score if current score is higher
    if (score > best_score) {
        best_score = score;
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Calculate where to position the game over message to center it
    // Adjusted for 4 lines below the board in total for messages
    int start_x = (cols - (BOARD_WIDTH + 2)) / 2;
    int start_y = (rows - (BOARD_HEIGHT + 2 + 4)) / 2; // Adjusted for 4 lines below

    // Ensure our starting points aren't negative
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;

    mvprintw(start_y + BOARD_HEIGHT / 2 - 2, start_x + BOARD_WIDTH / 2 - 5, "GAME OVER!");
    mvprintw(start_y + BOARD_HEIGHT / 2, start_x + BOARD_WIDTH / 2 - 10, "Your final score: %d", score);
    mvprintw(start_y + BOARD_HEIGHT / 2 + 1, start_x + BOARD_WIDTH / 2 - 10, "Best Score: %d", best_score); // Display best score on game over screen
    mvprintw(start_y + BOARD_HEIGHT / 2 + 3, start_x + BOARD_WIDTH / 2 - 20, "Press 'Enter' to play again, or 'q' to quit...");
    refresh();

    // Clear any pending input before waiting for the user's choice
    flushinp();
    // Disable non-blocking input to ensure getch() waits for user input
    nodelay(stdscr, FALSE); // This is equivalent to timeout(-1)

    int choice;
    do {
        choice = getch();
        // Loop until 'Enter' or 'q' (case-insensitive) is pressed
    } while (!(choice == '\n' || choice == KEY_ENTER || choice == 'q' || choice == 'Q'));

    // Re-enable non-blocking input for the game loop
    nodelay(stdscr, TRUE); // This is equivalent to timeout(0)

    // Check for Enter key (both '\n' and KEY_ENTER) or 'q' to quit
    if (choice == '\n' || choice == KEY_ENTER) {
        return 1; // Player wants to play again
    } else { // This else will only be reached if choice is 'q' or 'Q' due to the do-while loop
        return 0; // Player wants to quit
    }
}