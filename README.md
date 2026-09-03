# 🎮 Tic-Tac-Toe AI Core Engine with Minimax & Alpha-Beta Pruning

[![Language](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/Build-CMake%20%7C%20Ninja-orange.svg)](https://cmake.org/)
[![GUI](https://img.shields.io/badge/GUI-SDL2-green.svg)](https://www.libsdl.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

An extensible, high-performance zero-sum game engine developed in modern C++. The project features an intelligent game agent driven by the **Minimax algorithm with Alpha-Beta Pruning** and supports decoupled rendering layers (Terminal CLI and interactive SDL2 GUI) powered by object-oriented design patterns.

---

## 🚀 Key Features

- **Optimal Adversarial Decision Making:**
  - Implements recursive **Minimax** evaluation with **Alpha-Beta Pruning**, significantly reducing the search state space while ensuring optimal/unbeatable bot play.
  - Configurable bot difficulty levels (Easy, Medium, and Unbeatable/Hard).
- **Decoupled Architecture & Design Patterns:**
  - Follows **SOLID principles**, isolating game rules, state evaluation, and move generation from display logic.
  - Uses the **Strategy Pattern** for interchangeable rendering backends and bot policies.
- **Dual Interfaces:**
  - **Terminal / CLI Mode:** Fast, lightweight execution suitable for automated testing, benchmarks, and grading.
  - **SDL2 Graphical Interface:** Interactive 2D graphical rendering with event handling for human-vs-bot and bot-vs-bot matchups.
- **Automated Verification & Grading:**
  - Bundled with a Python grading and testing suite (`grader.py`) for automated test case verification and state regression testing.

---

## 🛠️ Tech Stack & Prerequisites

- **Language:** C++17
- **Build System:** CMake (>= 3.16), Ninja / Make
- **Libraries:** SDL2, SDL2_ttf (for font rendering)
- **Environment:** MSYS2 / MinGW-w64 (Windows) or GCC/Clang (Linux/macOS)
- **Testing:** Python 3.x

---

## 📁 Repository Structure

```text
.
├── CMakeLists.txt         # Root build configuration
├── grader.py              # Automated test harness & grading script
├── debug_input.txt        # Debugging / simulation input feed
├── testcase/              # Test suite inputs and expected outputs
├── src/                   # C++ Core engine and game logic implementation
│   ├── main.cpp           # Program entry point
│   ├── engine/            # Board representation, Minimax & Alpha-Beta solver
│   └── ui/                # Terminal and SDL2 renderer implementations
└── test-sdl/              # SDL2 prototyping and graphical test assets
⚙️ Build & Installation1. Clone the repositoryBashgit clone [https://github.com/hquan0938-web/level-2.git](https://github.com/hquan0938-web/level-2.git)
cd level-2
2. Build with CMakeBash# Generate build files
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# Compile the project
cmake --build build
(If using standard Makefiles, run cmake -B build && cmake --build build).🕹️ UsageRun Game EngineBash# Run Terminal / Interactive Mode
./build/tic_tac_toe

# Or run with test inputs
./build/tic_tac_toe < debug_input.txt
Automated Grading / VerificationRun the evaluation testbench to check state consistency against predefined test cases:Bashpython grader.py
```
## 🧠 Algorithmic Complexity

| Method | Time Complexity | Space Complexity | Decision Optimality |
| :--- | :--- | :--- | :--- |
| **Standard Minimax** | O(b^d) | O(d) | Guaranteed Optimal |
| **Alpha-Beta Pruning** | O(b^(d/2)) (Best) | O(d) | Guaranteed Optimal |

*Where `b <= 9` (branching factor) and `d <= 9` (maximum board depth).*
