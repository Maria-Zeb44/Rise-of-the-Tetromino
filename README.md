# Rise-of-the-Tetromino

Rise-of-the-Tetromino is a small C++ implementation of a Tetris-like game. It provides the core game loop, piece definitions, and basic gameplay mechanics for falling tetrominoes.

Features
- Classic tetromino shapes implemented as separate classes (`IShape`, `JShape`, `LShape`, `OShape`, `SShape`, `TShape`, `ZShape`).
- Game logic encapsulated in `Game.cpp` / `Game.h` with configurable constants in `GameConstants.h`.
- Simple, extendable codebase suitable for learning game programming in C++.

Repository structure
- `Game.cpp`, `Game.h` — main game logic and loop
- `Shape.h` and shape headers (`IShape.h`, `JShape.h`, `LShape.h`, `OShape.h`, `SShape.h`, `TShape.h`, `ZShape.h`) — tetromino definitions
- `main.cpp` — program entry point
- `GameConstants.h` — gameplay constants and configuration

Build & run (Windows)
You can build with a modern C++ compiler (tested with g++ / MinGW and MSVC).

Using g++ (MinGW):
```powershell
g++ -std=c++17 -O2 -o RiseOfTheTetromino main.cpp Game.cpp
.\RiseOfTheTetromino.exe
```

Using MSVC (Developer Command Prompt):
```powershell
cl /EHsc /std:c++17 /O2 main.cpp Game.cpp
RiseOfTheTetromino.exe
```

Gameplay
- Controls: use arrow keys to move and rotate pieces (project-specific keys may vary).
- Goal: complete horizontal lines to score and prevent the board from filling.

