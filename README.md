## OS – Multithreaded Ludo-Style Console Game (C++ / Windows)

**OS** is a multithreaded, console-based implementation of a Ludo-style board game written in C++ for Windows.  
It uses POSIX threads (`pthread`), mutexes, and condition variables to coordinate multiple player threads on a shared 15×15 board, with colored output in the Windows console.

---

### Features

- **Multithreaded gameplay**:
  - One thread per player (`player_thread`)
  - A dedicated **master thread** that monitors game state and declares the winner
- **4-color Ludo board**:
  - Colors: **Blue**, **Yellow**, **Red**, **Green**
  - Safe squares and start squares per color
  - Central winning area
- **Configurable setup**:
  - **2–4 players**
  - **1–4 tokens** per player
  - (Hooks for bot players are present, current logic uses `bot` flag)
- **Console visualization**:
  - 15×15 `char board[15][15]`
  - Windows console colors for board zones and tokens
- **Game mechanics**:
  - Dice rolls (with special rules for 6s)
  - Token movement along pre-defined paths per color
  - Safe squares, home yard, start squares, and winning tiles
  - Collision handling (sending opponent tokens back to yard)
  - Block formation detection (multiple same-color tokens on a square)
  - Basic AFK/removal logic for inactive players

---

### Project Structure

- **`OS.sln`**: Visual Studio solution
- **`OS/OS.vcxproj`**: Main C++ project
- **`OS/Source.cpp`**:
  - Entry point (`main`)
  - Board creation and display
  - Player and token structures
  - `PlayersListManager` singleton
  - Dice rolling and token movement logic
  - Multithreading (player threads + master thread)
  - Synchronization (mutexes, condition variables)
- **`OS/gameboard.cpp`** and **`OS/token.cpp`**:
  - Older / commented-out versions of board and token logic (reference only)
- **`x64/Debug/`**:
  - Built artifacts (`OS.exe`, PDB)
  - `pthreadVC3d.dll` for pthreads on Windows

---

### Requirements

- **Operating System**: Windows 10 or later
- **Toolchain**:
  - Visual Studio (2019 or later recommended) with **Desktop development with C++** workload
  - `pthread` for Windows (already referenced via `pthreadVC3d.dll` in `x64/Debug`)
- **Language**: C++ (uses standard library plus `windows.h` and `pthread`)

---

### Building the Project

#### Using Visual Studio

1. **Open the solution**
   - Double-click `OS.sln` or
   - In Visual Studio: **File → Open → Project/Solution…** and select `OS.sln`.

2. **Select configuration**
   - In the toolbar, choose:
     - **Configuration**: `Debug`
     - **Platform**: `x64`

3. **Build**
   - Press **Ctrl+Shift+B** or
   - **Build → Build Solution**

If everything is configured correctly, `OS.exe` will be generated in `x64/Debug/`.

---

### Running the Game

1. Ensure that:
   - `OS.exe`
   - `pthreadVC3d.dll`  
     are both present in the same folder (typically `x64/Debug`).

2. Run from Visual Studio:
   - Set `OS` as the **Startup Project**
   - Press **F5** (Debug) or **Ctrl+F5** (Run without debugging)

3. Or run from a terminal (PowerShell / CMD):

   cd path\to\project\x64\Debug
   .\OS.exe
