# Tetris in C++

## Description
This project is a simple Tetris clone implemented in C++ using the ncurses library. It features basic Tetris gameplay with falling tetrominoes, collision detection, line clearing, scoring, and level progression. The game is rendered in a console window and utilizes non-blocking input to provide a smooth playing experience.

## Controls
- **Right Arrow**: Move the current piece to the right.
- **Left Arrow**: Move the current piece to the left.
- **Down Arrow**: Accelerate the descent of the current piece.
- **Up Key**: Rotate the current piece (clockwise).

## Features
- **Tetromino Rotation**: Pieces are rotated using a custom rotation function.
- **Collision Detection**: Checks if a tetromino can fit in a specified position without overlapping existing blocks.
- **Dynamic Speed**: The falling speed increases after every 10 pieces, adding to the challenge.
- **Scoring System**: Implements a scoring system similar to classic Tetris, awarding points based on the number of lines cleared at once.
- **Line Clearing**: Completed lines are detected, cleared, and the above rows are shifted down.
- **Level Progression**: The game level increases as more lines are cleared, which also increases the game speed.

## Code Structure
- **Global Variables**: Define the screen dimensions, field size, tetromino shapes, and game state variables.
- **Functions**:
  - `Rotate(int px, int py, int r)`: Computes the new index for a tetromino cell after a rotation.
  - `DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)`: Checks if the current tetromino fits in the field at the given position and rotation.
- **Main Function**:
  - **Initialization**: Sets up ncurses, initializes the game field and tetromino shapes.
  - **Game Loop**: Contains the main game logic for input handling, piece movement, collision checking, line clearing, scoring, and level progression.
  - **Rendering**: Uses ncurses functions to render the game field and the current tetromino.
  - **Game Over**: Ends the game when no new piece can be placed, then displays the final score.

## Compile and Execution
1. **Prerequisites**: Ensure you have a C++ compiler (e.g., g++) and the ncurses library installed.
2. **Compilation**: Open a terminal and run the following command:
   ```bash
   g++ -std=c++11 -lncurses -pthread -o tetris tetris.cpp
