<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
 
</head>
<body>

  <h1>🐍 C Snake Game</h1>
  <p>A classic terminal-based Snake game implemented in <strong>C</strong> using the <code>ncurses</code> library for rendering and input handling.</p>

  <h2>📋 Prerequisites</h2>
  <ul>
    <li>C Compiler (e.g., GCC)</li>
    <li>ncurses library</li>
  </ul>

  <h2>📦 Installing <code>ncurses</code></h2>

  <div class="os">💻 Debian/Ubuntu (Linux)</div>
  <pre><code>sudo apt-get update
sudo apt-get install libncurses5-dev libncursesw5-dev</code></pre>

  <div class="os">🐧 Fedora/RHEL (Linux)</div>
  <pre><code>sudo dnf install ncurses-devel</code></pre>

  <div class="os">🍏 macOS (Homebrew)</div>
  <pre><code>brew install ncurses</code></pre>

  <div class="os">🪟 Windows (MSYS2)</div>
  <pre><code>pacman -S mingw-w64-x86_64-ncurses
# or for 32-bit
pacman -S mingw-w64-i686-ncurses</code></pre>

  <div class="note">
    💡 Make sure your environment variables (e.g., <code>PATH</code>) are correctly set for your compiler.
  </div>

  <h2>⚙️ How to Compile and Run</h2>
  <ol>
    <li>Save the C code into a file named <code>snake.c</code>.</li>
    <li>Open your terminal and navigate to the file's directory.</li>
    <li>Compile the code using:
      <pre><code>gcc snake.c -o snake -lncurses</code></pre>
    </li>
    <li>Run the game:
      <pre><code>./snake</code></pre>
    </li>
  </ol>

  <h2>🎮 How to Play</h2>
  <ul>
    <li>Use the <strong>Arrow Keys</strong> to move the snake.</li>
    <li>Eat <code>*</code> (food) to grow longer and earn points.</li>
    <li>Avoid hitting the <code>#</code> walls or your own <code>o</code> body.</li>
    <li>When the game ends:
      <ul>
        <li>Press <strong>Enter</strong> to restart</li>
        <li>Press <strong>q</strong> to quit</li>
      </ul>
    </li>
  </ul>

  <h2>✨ Features</h2>
  <ul>
    <li>Terminal-based real-time rendering</li>
    <li>Responsive snake movement</li>
    <li>Self-collision and border detection</li>
    <li>Replayability</li>
  </ul>

  <h2>💡 Tip</h2>
  <p>You can extend this game by adding levels, color effects, score saving, or even multiplayer mode!</p>
  <p>fork the project and add more features if you can</p>

</body>
</html>
