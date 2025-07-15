
C Snake Game
This is a classic Snake game implemented in C, designed to be played in a terminal environment.

Prerequisites
To compile and run this game, you will need:

A C Compiler: Most commonly, GCC (GNU Compiler Collection). If you're on Linux or macOS, it's usually pre-installed or easily installable. For Windows, MinGW-w64 is a popular choice.

ncurses Library: This library provides the functionality for text-based user interfaces, allowing the game to draw on the terminal and handle input without waiting for Enter after every key press.

Installing ncurses
The installation method for ncurses varies depending on your operating system:

On Debian/Ubuntu (Linux):

sudo apt-get update
sudo apt-get install libncurses5-dev libncursesw5-dev

On Fedora/RHEL (Linux):

sudo dnf install ncurses-devel

On macOS (using Homebrew):

brew install ncurses

On Windows (using MinGW/MSYS2):
If you're using MinGW or MSYS2, you'll typically install ncurses through their package managers. For MSYS2, you might use:

pacman -S mingw-w64-x86_64-ncurses # For 64-bit MinGW
# or
pacman -S mingw-w64-i686-ncurses  # For 32-bit MinGW

Ensure your MinGW environment is correctly set up and its bin directory is in your system's PATH.

How to Compile and Run
Save the Code:
Save the C code (e.g., from the snake-game-c Canvas) into a file named snake.c.

Open Your Terminal:
Navigate to the directory where you saved snake.c.

Compile the Game:
Use your C compiler (GCC is assumed here) and link against the ncurses library:

gcc snake.c -o snake -lncurses

gcc: The C compiler.

snake.c: Your source code file.

-o snake: Specifies the output executable file name as snake.

-lncurses: Links against the ncurses library.

Run the Game:
Once compiled successfully, execute the game:

./snake

How to Play
Use the Arrow Keys (Up, Down, Left, Right) to control the snake's direction.

The snake moves continuously.

Eat the * (food) to grow longer and increase your score.

Avoid hitting the # borders or the snake's own o body.

When the game ends, press Enter to play again, or 'q' to quit.