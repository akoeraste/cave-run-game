# Cave Run Game

## 📖 Project Overview

**Cave Run** is a turn-based console adventure game developed in C++ as part of an Object-Oriented Analysis and Design (OOAD) course project. The game demonstrates key object-oriented programming principles including inheritance, polymorphism, abstraction, and encapsulation.

### Game Objective

You play as a character (marked **P**) who must escape from a dangerous cave. Your goal is to navigate through a grid-based map, avoid hazards, and evade a malicious monster (marked **M**) to reach the exit located at the top-right corner. The player starts at the bottom-left corner of the cave.

---

## 🎮 Game Features

- **Turn-based gameplay**: Player moves first, then the monster
- **Multiple room types**: Safe rooms, poison rooms, and trap rooms
- **Health system**: Player starts with 100 HP and can take damage from hazards
- **Poison mechanics**: Poisoned players take damage over time with a chance to cure
- **Monster AI**: Intelligent monster that adapts its strategy based on player position
- **Random map generation**: Each game features a randomly generated map layout
- **Real-time status display**: Health, distances, and game state always visible

---

## 📋 Table of Contents

1. [Prerequisites](#prerequisites)
2. [Project Structure](#project-structure)
3. [Installation & Compilation](#installation--compilation)
4. [How to Play](#how-to-play)
5. [Game Mechanics](#game-mechanics)
6. [Architecture & Design](#architecture--design)
7. [Class Documentation](#class-documentation)
8. [Troubleshooting](#troubleshooting)
9. [Technical Details](#technical-details)
10. [Future Improvements](#future-improvements)

---

## 🔧 Prerequisites

### Required Software

- **C++ Compiler**: 
  - **Windows**: Visual Studio 2019 or later, MinGW-w64, or Clang
  - **Linux**: GCC 7.0+ or Clang 8.0+
  - **macOS**: Xcode Command Line Tools or Clang
- **C++ Standard**: C++14 or higher (C++17 recommended)
- **Operating System**: Windows, Linux, or macOS
- **Terminal/Command Prompt**: For running the console application

### Knowledge Requirements

- Basic understanding of C++ syntax
- Familiarity with object-oriented programming concepts
- Understanding of inheritance and polymorphism (helpful but not required to play)

---

## 📁 Project Structure

```
Cave Run/
│
├── Headers/                    # Header files (.h)
│   ├── Position.h              # 2D coordinate system
│   ├── Room.h                  # Abstract base class for rooms
│   ├── NormalRoom.h            # Safe rooms
│   ├── PoisonRoom.h            # Poison hazard rooms
│   ├── TrapRoom.h              # Trap hazard rooms
│   ├── Character.h             # Abstract base class for characters
│   ├── Player.h                # Player character implementation
│   ├── Monster.h               # Monster character implementation
│   ├── Map.h                   # Game map/grid management
│   └── Game.h                  # Main game controller
│
├── Source/                     # Implementation files (.cpp)
│   ├── Position.cpp
│   ├── Room.cpp
│   ├── NormalRoom.cpp
│   ├── PoisonRoom.cpp
│   ├── TrapRoom.cpp
│   ├── Character.cpp
│   ├── Player.cpp
│   ├── Monster.cpp
│   ├── Map.cpp
│   ├── Game.cpp
│   └── main.cpp                       # Program entry point
│
├── docs/
│   ├── ooad work.pdf                  # Assignment documentation
│   └── OOADP CAVE RUN GAME docs.pdf   # Project document
│
├── starting.md                        # Implementation guide
└── README.md                          # This file
```

### File Organization

- **Headers/**: Contains all class declarations and interfaces
- **Source/**: Contains all class implementations
- This separation follows the **declaration/definition split** pattern in C++

---

## 🚀 Installation & Compilation

### Method 1: Command Line (GCC/Clang)

#### Windows (MinGW/MSYS2)

```bash
cd "CaveRun"
g++ -std=c++17 -IHeaders Source/*.cpp -o CaveRun.exe
```

#### Linux/macOS

```bash
cd /path/to/Cave\ Run
g++ -std=c++17 -IHeaders Source/*.cpp -o CaveRun
```

### Method 2: Visual Studio (Windows)

1. Open Visual Studio
2. Create a new **C++ Console App** project
3. Add all files from `Headers/` and `Source/` folders to the project
4. Right-click project → **Properties** → **C/C++** → **General**
   - Add `Headers` folder to **Additional Include Directories**
5. Set C++ Language Standard to **C++17** or higher
6. Build and run (F5 or Ctrl+F5)

### Method 3: CMake (Cross-platform)

Create a `CMakeLists.txt` file:

```cmake
cmake_minimum_required(VERSION 3.10)
project(CaveRun)

set(CMAKE_CXX_STANDARD 17)

include_directories(Headers)

file(GLOB_RECURSE SOURCES "Source/*.cpp")

add_executable(CaveRun ${SOURCES})
```

Then compile:

```bash
mkdir build
cd build
cmake ..
make
```

### Running the Game

After compilation:

```bash
# Windows
CaveRun.exe

# Linux/macOS
./CaveRun
```

---

## 🎯 How to Play

### Controls

| Key | Action |
|-----|--------|
| **W** or **w** | Move Up (North) |
| **S** or **s** | Move Down (South) |
| **A** or **a** | Move Left (West) |
| **D** or **d** | Move Right (East) |

### Gameplay Flow

1. **Start**: Game begins with player at bottom-left (0,0) and monster at top-right exit
2. **Player Turn**: 
   - You can make up to **2 moves** per turn
   - Enter W/A/S/D to move in that direction
   - After each move, the game displays updated information
3. **Monster Turn**: 
   - Monster automatically moves once after your turn
   - Monster uses AI to either chase you or guard the exit
4. **Win Condition**: Reach the exit (top-right corner)
5. **Lose Conditions**: 
   - Health reaches 0
   - Monster catches you (same position)

### Understanding the Map

```
. = Normal/Safe Room
? = Unknown Room (may be hazardous)
T = Trap Room (visited, dangerous)
P = Poison Room (visited, dangerous)
P = Player (blue in GUI versions)
M = Monster (red in GUI versions)
```

**Note**: In the console version, 'P' appears as both the Player symbol and Poison Room symbol. The context (whether M or other symbols are nearby) helps distinguish them.

---

## 🎲 Game Mechanics

### Health System

- **Initial Health**: 100 HP
- **Trap Rooms**: Deal **20 damage** immediately and end your turn
- **Poison Rooms**: Apply poison status (5 damage per turn)
- **Death**: Game ends if health reaches 0

### Poison System

- **Effect**: Player loses **5 HP** at the end of each turn while poisoned
- **Cure Chance**: 20% chance to cure poison each turn
- **Visual Indicator**: Health display shows `[POISONED]` status

### Room Types

#### Normal Room (`.`)
- **Effect**: No damage or special effects
- **Symbol**: `.` (always visible)

#### Poison Room (`?` → `P`)
- **Unvisited**: Shown as `?`
- **Visited**: Shown as `P`
- **Effect**: 
  - Applies poison status to player
  - Deals 5 damage per turn until cured
  - Room becomes visible after first visit

#### Trap Room (`?` → `T`)
- **Unvisited**: Shown as `?`
- **Visited**: Shown as `T`
- **Effect**: 
  - Deals 20 damage immediately
  - Ends player's turn immediately
  - Room becomes visible after first visit

### Monster AI

The monster uses strategic AI:

1. **Distance Calculation**: Compares its distance to exit vs. player's distance to exit
2. **Decision Logic**:
   - If monster is **closer to exit** than player → **Chase player**
   - If player is **closer to exit** than monster → **Guard exit** (move toward exit)
3. **Movement**: Moves **one step at a time** (horizontal preferred, then vertical)
4. **No Diagonal Movement**: Monster moves orthogonally only

### Map Generation

- **Size**: 8x8 grid (configurable in `main.cpp`)
- **Distribution**: 
  - 20% Poison Rooms
  - 20% Trap Rooms
  - 60% Normal Rooms
- **Safety**: Start and Exit positions are always safe (Normal Rooms)
- **Randomization**: Uses seeded random number generator (time-based seed)
- **Reproducibility**: Same seed produces same map layout

---

## 🏗️ Architecture & Design

### Object-Oriented Design Principles

This project demonstrates several OOP principles:

1. **Inheritance**: 
   - `Character` → `Player`, `Monster`
   - `Room` → `NormalRoom`, `PoisonRoom`, `TrapRoom`

2. **Polymorphism**: 
   - `Room::visit()` and `Room::getSymbol()` are virtual functions
   - `Character::sense()`, `Character::decideMove()`, `Character::update()` are pure virtual

3. **Encapsulation**: 
   - Private data members with public getters/setters
   - Internal implementation hidden from users

4. **Abstraction**: 
   - Abstract base classes define interfaces
   - Derived classes implement specific behaviors

### Design Patterns

- **Template Method Pattern**: `Character` defines move flow, subclasses customize behavior
- **Strategy Pattern**: Monster AI uses different strategies based on game state
- **Factory Pattern**: `Map::initialize()` creates different room types

### Class Relationships

```
Game
├── Map (has many)
│   └── Room* (polymorphic collection)
│       ├── NormalRoom
│       ├── PoisonRoom
│       └── TrapRoom
├── Player (is-a Character)
└── Monster (is-a Character)
    └── uses Position
```

---

## 📚 Class Documentation

### Position

**Purpose**: Represents a 2D coordinate in the game grid.

**Methods**:
- `Position(int x, int y)`: Constructor
- `distanceTo(const Position& other)`: Calculates Manhattan distance
- `isAdjacentTo(const Position& other)`: Checks if positions are adjacent (including diagonals)

**Usage**: Used throughout the game for tracking locations.

---

### Room (Abstract Base Class)

**Purpose**: Base class for all room types in the game.

**Methods**:
- `getPosition() const`: Returns room's position
- `isVisited() const`: Checks if player has visited
- `markVisited()`: Marks room as visited
- `getSymbol() const` **(pure virtual)**: Returns display symbol
- `visit(Player& player)` **(pure virtual)**: Applies room effect to player

**Derived Classes**:
- `NormalRoom`: Safe passage
- `PoisonRoom`: Applies poison
- `TrapRoom`: Deals damage and ends turn

---

### Character (Abstract Base Class)

**Purpose**: Base class for all moving entities (Player, Monster).

**Attributes**:
- `position`: Current location
- `remainingMoves`: Moves left in current turn
- `maxMovesPerTurn`: Maximum moves per turn

**Methods**:
- `getPosition() const`: Returns current position
- `setPosition(const Position& pos)`: Sets position
- `resetMoves()`: Resets moves to maximum
- `hasMovesLeft() const`: Checks if can move
- `move(const Position& target, const Map& map)`: Moves character
- `sense(const Game& game)` **(pure virtual)**: Gathers information
- `decideMove(const Game& game)` **(pure virtual)**: Decides next move
- `update()`: Called after each move (default: no-op)

---

### Player

**Purpose**: Represents the human player character.

**Attributes**:
- `health`: Current health points (default: 100)
- `poisoned`: Poison status flag
- `poisonDamage`: Damage per turn when poisoned (default: 5)
- `cureProbability`: Chance to cure poison each turn (default: 0.2 = 20%)

**Methods**:
- `damage(int amount)`: Reduces health
- `getHealth() const`: Returns current health
- `isDead() const`: Checks if health <= 0
- `setPoisoned(bool flag)`: Sets poison status
- `isPoisoned() const`: Returns poison status
- `setPoisonDamage(int dmg)`: Sets poison damage amount
- `update()`: Applies poison damage and attempts cure

**Special Behavior**:
- Player gets **2 moves per turn**
- Player movement is controlled by user input (WASD)
- Automatically visits rooms when entering them

---

### Monster

**Purpose**: Represents the AI-controlled antagonist.

**Attributes**:
- Inherits from `Character`
- Moves **1 time per turn**

**Methods**:
- `sense(const Game& game)`: Analyzes game state
- `decideMove(const Game& game)`: Implements AI strategy

**AI Behavior**:
- Compares distances to exit (monster vs. player)
- If monster closer to exit → moves toward player (attack mode)
- If player closer to exit → moves toward exit (guard mode)
- Moves orthogonally (one step at a time, no diagonals)

---

### Map

**Purpose**: Manages the game grid and room collection.

**Attributes**:
- `width`, `height`: Grid dimensions
- `startPos`: Starting position (bottom-left)
- `exitPos`: Exit position (top-right)
- `rooms`: 2D vector of Room pointers

**Methods**:
- `initialize()`: Generates random map layout
- `isInside(const Position& pos)`: Checks bounds
- `isAdjacent(const Position& from, const Position& to)`: Validates adjacency
- `getRoom(const Position& pos)`: Returns room at position
- `getStartPosition()`, `getExitPosition()`: Position getters
- `getWidth()`, `getHeight()`: Dimension getters

**Memory Management**:
- Destructor properly deallocates all Room objects
- Prevents memory leaks

---

### Game

**Purpose**: Main game controller and orchestrator.

**Attributes**:
- `map`: Game map instance
- `player`: Player character instance
- `monster`: Monster character instance
- `gameOver`: Game end flag
- `playerWon`: Victory condition flag

**Methods**:
- `start()`: Initializes game state
- `run()`: Main game loop
- `processPlayerTurn()`: Handles player input and movement
- `processMonsterTurn()`: Executes monster AI and movement
- `checkGameOver()`: Evaluates win/lose conditions
- `getPlayerPosition()`, `getMonsterPosition()`, `getExitPosition()`: Position queries
- `render()`: Displays game state (map, health, distances)
- `showGameOver()`: Displays end-game message

**Game Loop Flow**:
1. Render game state
2. Process player turn (up to 2 moves)
3. Check game over conditions
4. Process monster turn (1 move)
5. Check game over conditions
6. Repeat until game ends

---

## 🔧 Troubleshooting

### Common Compilation Errors

#### Error: "Cannot find header file"
**Solution**: 
- Ensure include paths are set correctly
- For command line: Use `-IHeaders` flag
- For IDE: Add `Headers/` to include directories

#### Error: "Multiple definition of..."
**Solution**: 
- Ensure header files use include guards (`#ifndef`, `#define`, `#endif`)
- Don't include `.cpp` files directly

#### Error: "Undefined reference to..."
**Solution**: 
- Make sure all `.cpp` files are compiled
- Check that all source files are included in compilation command

### Runtime Issues

#### Game crashes on start
- Check that `Map::initialize()` is called before accessing rooms
- Verify map dimensions are valid (positive integers)

#### Invalid input handling
- Game now uses WASD controls (case-insensitive)
- Invalid keys show error message and prompt again
- Movement outside bounds is prevented

#### Monster collision false positives
- **Fixed**: Monster now moves one step at a time (no diagonal)
- Collision only occurs when positions are exactly equal
- Monster movement validated before execution

---

## 💡 Technical Details

### Memory Management

- **Smart Pointers**: Could be improved with `std::unique_ptr` or `std::shared_ptr`
- **Current Approach**: Manual memory management with destructors
- **RAII**: Resources (Room objects) are properly cleaned up in Map destructor

### Random Number Generation

- **Generator**: `std::mt19937` (Mersenne Twister)
- **Seed**: Time-based for variety across runs
- **Distribution**: Uniform real distribution [0.0, 1.0)
- **Reproducibility**: Same seed produces same sequence

### Coordinate System

- **Origin**: Bottom-left corner is (0, 0)
- **X-axis**: Increases rightward (West → East)
- **Y-axis**: Increases upward (South → North)
- **Distance**: Manhattan distance (L1 norm)
- **Adjacency**: Includes diagonals (8-directional)

### Input System

- **Method**: Character input via `std::cin`
- **Validation**: Switch statement with case-insensitive handling
- **Buffer Clearing**: `std::cin.ignore()` prevents input issues
- **Error Recovery**: Invalid input doesn't consume turn

---

## 🚀 Future Improvements

### Potential Enhancements

1. **GUI Version**:
   - Graphical interface using SFML, SDL, or Qt
   - Colored rooms and characters
   - Visual feedback for actions

2. **More Room Types**:
   - Healing rooms (restore health)
   - Teleport rooms (move to random location)
   - Secret passages

3. **Game Features**:
   - Multiple difficulty levels
   - Different map sizes
   - Save/Load game functionality
   - Score system based on moves/health
   - Multiple levels/rounds

4. **Code Improvements**:
   - Smart pointers for memory management
   - Configuration file for game parameters
   - Unit tests for core functionality
   - Better error handling with exceptions

5. **AI Enhancements**:
   - More sophisticated monster AI
   - Different monster types
   - Player difficulty options

6. **Documentation**:
   - UML diagrams generation
   - Code comments enhancement
   - API documentation (Doxygen)

---

## 📖 Educational Value

### Learning Outcomes

This project demonstrates:

1. **Object-Oriented Programming**:
   - Class hierarchies and inheritance
   - Virtual functions and polymorphism
   - Encapsulation and abstraction

2. **Software Design**:
   - Separation of concerns
   - Modular architecture
   - Design patterns application

3. **C++ Best Practices**:
   - Header/implementation separation
   - Const correctness
   - Memory management awareness

4. **Game Development**:
   - Game loop structure
   - State management
   - User input handling

### Recommended Study Areas

- **Inheritance**: Study how `Character` and `Room` hierarchies work
- **Polymorphism**: Observe virtual function calls in room visits
- **Design Patterns**: Identify Strategy, Template Method patterns
- **Memory Management**: Understand Map's destructor role

---

## 📄 License & Credits

### Original Assignment

This project was created as part of an Object-Oriented Analysis and Design (OOAD) course assignment. The game concept and requirements are based on the provided course materials.

### Development Notes

- **Language**: C++17
- **Paradigm**: Object-Oriented Programming
- **Platform**: Cross-platform (Windows, Linux, macOS)
- **Build System**: Manual compilation or CMake

---

## 📞 Support & Contact

### Getting Help

1. **Check Troubleshooting Section**: Common issues and solutions above
2. **Review Code Comments**: Inline documentation in source files
3. **Study Class Documentation**: Each class's purpose and methods

### Contributing

This is an educational project. Suggestions for improvements are welcome:
- Bug reports
- Code optimization ideas
- Feature suggestions
- Documentation improvements

---

## 📚 References

### Documentation Files

- `docs/ooad work.pdf`: Original assignment requirements and specifications
- `docs/OOADP CAVE RUN GAME docs.pdf`: Detailed project documentation
- `starting.md`: Implementation guide and code structure

### Recommended Reading

- C++ Object-Oriented Programming concepts
- Design Patterns (Gang of Four)
- Game Loop architecture patterns
- C++ Standard Library documentation

---

## 🎓 Conclusion

**Cave Run** is a comprehensive implementation of a turn-based adventure game that serves as an excellent demonstration of object-oriented programming principles. The codebase is well-organized, modular, and follows best practices for C++ development.

Whether you're a beginner learning OOP concepts or an experienced developer reviewing design patterns, this project offers valuable insights into creating maintainable, extensible software architecture.

**Happy coding and happy escaping!** 🎮

---

*Last Updated: 2026*
*Version: 1.0*
*Status: Complete and Functional*
