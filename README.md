Buzz Bomber is a high-energy arcade shooter where you take control of a sprayer 🔫 to defend your beautiful garden 🌸 from relentless swarms of bees and hornets 🐝💨. It’s all about quick reflexes, precise shooting, and staying one step ahead of the chaos!

## How It Works

The game is divided into progressively harder levels, each designed to test your skills and keep you on your toes 🚀:

- **Early Levels**: Start with chill vibes 😎. Small, manageable swarms give you time to master the controls and practice your aim.
- **Mid Levels**: Things heat up 🔥. Faster bees and hornets enter the fray, requiring quick thinking to dodge stingers and protect your flowers.
- **Later Levels**: Pure chaos 🫠. Waves of relentless swarms come at you from all sides, forcing you to multitask like a boss 💪. Every second counts as you defend your garden.

With no power-ups to rely on, it’s all about skill and strategy 🧠✨. Can you survive the buzz and keep your garden blooming?

---

## How to Compile the Game (with SFML)

To run the game on your system, you need to install the SFML library, which handles the graphics, audio, and input for the game. Here’s how to compile your C++ file based on your operating system:

### 1. macOS 🍎
1. Install SFML using Homebrew:
   ```bash
   brew install sfml
   ```
2. Compile the game:
   ```bash
   g++ -o BuzzBomber game.cpp -lsfml-graphics -lsfml-window -lsfml-system
   ```
3. Run the game:
   ```bash
   ./BuzzBomber
   ```

---

### 2. Linux 🐧
1. Install SFML using your package manager (for Ubuntu/Debian):
   ```bash
   sudo apt-get install libsfml-dev
   ```
2. Compile the game:
   ```bash
   g++ -o BuzzBomber game.cpp -lsfml-graphics -lsfml-window -lsfml-system
   ```
3. Run the game:
   ```bash
   ./BuzzBomber
   ```

---

### 3. Windows 🪟
1. Download and install SFML from [sfml-dev.org](https://sfml-dev.org).
2. Set up your compiler (e.g., MinGW) and link the SFML libraries.
3. Compile using:
   ```bash
   g++ -o BuzzBomber game.cpp -lsfml-graphics -lsfml-window -lsfml-system
   ```
4. Run the executable:
   ```bash
   BuzzBomber.exe
   ```

---

With SFML installed, you’re all set to jump into the action-packed world of Buzz Bomber! 🐝🎮
```
