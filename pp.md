Main Classes and Responsibilities
From the requirements and use cases, the key design classes and their responsibilities are as follows.
•	Game
o	Orchestrates a complete game session (initialization, turn loop, and termination).
o	Creates and owns the Map, Player, and Monster objects.
o	Alternates turns between Player and Monster, invokes character actions (sense, move, update), and checks for win or loss conditions.
•	Map
o	Represents the 2D grid of rooms forming the cave.
o	Stores references to Room objects in a 2D structure and provides lookup operations by grid coordinates.
o	Knows the grid dimensions and the positions of the start and exit rooms to support boundary checks and win-condition evaluation.
•	Room (abstract)
o	Base type for all room variants.
o	Stores its grid position and display attributes such as color or label.
o	Declares a virtual visit(Player) operation that is overridden by subclasses to implement specific effects when the player enters the room.
•	NormalRoom (subclass of Room)
o	Represents a safe room with no special effect.
o	Overrides visit(Player) without changing health or status, leaving the player unchanged except for position.
•	PoisonRoom (subclass of Room)
o	Represents a poisonous room that poisons the player on visit.
o	Overrides visit(Player) to mark the player as poisoned and possibly configure ongoing damage per update; the room’s display is changed (e.g. to green) after being visited to indicate hazard.
•	TrapRoom (subclass of Room)
o	Represents a trap that inflicts a fixed amount of damage and immediately ends the player’s turn.
o	Overrides visit(Player) to subtract health and set the player’s remaining moves to zero; the room’s display is changed (e.g. to red) after being visited.
•	Character (abstract)
o	Base type for movable entities (Player and Monster).
o	Stores current position and the number of remaining moves in the current turn.
o	Provides generic movement logic and declares abstract or overridable operations for sensing game state, deciding movement direction, and performing per-turn updates. 
•	Player (subclass of Character)
o	Adds attributes for health points, poison status, poison damage per update, and probability of curing poison.
o	Moves based on user input and tracks the rooms visited; can make up to two moves per turn, with this allowance reset at the beginning of each player turn.
o	In update(), applies ongoing poison damage if poisoned and attempts to cure poison according to the configured probability.
•	Monster (subclass of Character)
o	Moves autonomously by sensing both the player’s position and the exit room position.
o	On its turn, compares its distance to the exit with the player’s distance to the exit; if the monster is closer to the exit, it moves towards the player, otherwise it moves towards the exit, advancing one step per turn.
•	Position
o	Represents an (x, y) coordinate on the map grid using integer components.
o	Used by both Room and Character to identify locations and compute adjacency and distances.
•	HealthStatus
o	Represents the player’s health points and high-level status flags such as “poisoned” or “dead”.
o	Provides simple operations to reduce health, check for zero or below, and update status.
Each class has a focused responsibility, which helps keep the design modular and easier to extend, for example by adding new room types or alternative character behaviors.




Mapping Design Classes to C++ Code
Each UML class from Figure 3.1 is implemented as a C++ class, usually with separate header and source files for clarity and modularity.
The key source files include Game, Map, the Character hierarchy (Character, Player, Monster), the Room hierarchy (Room, NormalRoom, PoisonRoom, TrapRoom), and basic helper classes like Position.
A typical organization is:
•	Game.h / Game.cpp: game control, initialization, main loop, and game-over checks.
•	Map.h / Map.cpp: room grid, coordinate handling, and start/exit positions.
•	Character.h, Player.h, Monster.h (+ corresponding .cpp files): movement logic and per-turn updates.
•	Room.h, NormalRoom.h, PoisonRoom.h, TrapRoom.h (+ .cpp): polymorphic room effects invoked from the game loop.
This structure closely follows the UML class diagram, making it easy to navigate from documentation to code and back.
4.3 Implementation of Core Classes
4.3.1 Character, Player, and Monster
The abstract base class Character encapsulates common data and behavior for movable entities, while Player and Monster provide specialized implementations.
Snippet 4.1: Character base class and Player inheritance
// Character.h
class Game;      
class Map;
class Position;

class Character {
protected:
    Position position;
    int remainingMoves;

public:
    Character(const Position& startPos, int movesPerTurn);
    virtual ~Character() = default;

    const Position& getPosition() const;
    void setPosition(const Position& pos);

    void resetMoves();
    bool hasMovesLeft() const;

    virtual void sense(const Game& game) = 0;
    virtual Position decideMove(const Game& game) = 0;
    virtual void update() {}          

    void move(const Position& target, const Map& map);
};

//Player.h
class Player : public Character {
private:
    int health;
    bool poisoned;
    int poisonDamage;
    double cureProbability;

public:
    Player(const Position& startPos);

    void damage(int amount);
    bool isDead() const;

    void setPoisoned(bool flag);
    bool isPoisoned() const;

    void sense(const Game& game) override;
    Position decideMove(const Game& game) override;
    void update() override;           // applies poison damage and cure chance
};

The Player inherits from Character and overrides sense, decideMove, and update to implement user-controlled movement and poison effects, while reusing the base movement logic. 
The Monster class similarly inherits from Character and overrides decision logic to implement the distance-based chase behavior described in Chapter 3.
4.3.2 Room Hierarchy and Room Effects
The Room hierarchy implements different room behaviors using polymorphism through a virtual visit(Player&) function.
Snippet 4.2 Room base class and specialized room effects
//room.h
class Room {
protected:
    Position position;
    bool visited;

public:
    Room(const Position& pos);
    virtual ~Room() = default;

    const Position& getPosition() const;
    bool isVisited() const;
    void markVisited();

    virtual void visit(Player& player) = 0;  // pure virtual
};

// PoisonRoom.h
class PoisonRoom : public Room {
private:
    int poisonDamage;

public:
    PoisonRoom(const Position& pos, int dmg);

    void visit(Player& player) override;     // poisons the player
};

// PoisonRoom.cpp
void PoisonRoom::visit(Player& player) {
    player.setPoisoned(true);
    // configure ongoing damage if needed
    markVisited();
}

// TrapRoom.h
class TrapRoom : public Room {
private:
    int damageAmount;

public:
    TrapRoom(const Position& pos, int dmg);

    void visit(Player& player) override;     // damage and end turn
};

// TrapRoom.cpp
void TrapRoom::visit(Player& player) {
    player.damage(damageAmount);
    player.resetMoves();    // or set remainingMoves to 0 to end turn
    markVisited();
}

At runtime, the Game class holds pointers or references to Room but calls visit polymorphically, which ensures the correct effect is applied without type checks. 
4.3.3 Game and Map
The Game class implements the main loop and delegates map- and room-related tasks to Map and the room hierarchy.
Snippet 4.3 Using polymorphism in the game loop
// Game.cpp
void Game::processPlayerTurn() {
    while (!gameOver && player.hasMovesLeft()) {
        // get movement command from UI
        Position target = ui->getPlayerMove();

        if (!map.isInside(target) 
		|| !map.isAdjacent(player.getPosition(), target)) {
            ui->showError("Invalid move");
            continue;
        }

        Room* room = map.getRoom(target);
        player.move(target, map);

        room->visit(player); // polymorphic call

        player.update();     // apply poison and other effects
        checkGameOver();

        ui->refresh(map, player, monster);
        if (!player.hasMovesLeft() || gameOver) break;
    }
}
The Map class manages room storage and coordinate checks, typically using a 2D container such as std::vector<std::vector<Room*>>, and provides helper methods like isInside, getRoom, and accessors for start and exit positions.

4.4 User Interface Implementation
The user interface layer interacts with the Game class through a small, well-defined set of methods, without directly manipulating internal game data structures.
For console-based implementations, a typical pattern is:
•	Read user input (e.g. “W/A/S/D”, or coordinates).
•	Translate input into a Position or direction and pass it to Game methods such as processPlayerTurn.
•	After each action, request the current map and status from Game and redraw the grid and text-based status. 
In a graphical implementation, the UI would run an event loop that captures keyboard or mouse events, calls Game operations, and uses drawing functions to render the updated map and entities in a window. 

4.5 Use of Inheritance, Polymorphism, and Encapsulation
Cave Run’s implementation demonstrates object-oriented principles directly tied to the UML design.
•	Inheritance:
o	Player and Monster inherit from Character, sharing movement and state while customizing sensing and decision logic.
o	NormalRoom, PoisonRoom, and TrapRoom inherit from Room, sharing position and visited state while customizing visit(Player&).
•	Polymorphism:
o	The visit function is called through Room* in the game loop, so the effect depends on whether the concrete room is normal, poisonous, or a trap. 
o	The update and decision methods of Character subclasses are invoked via base-class pointers, enabling different behaviors for Player and Monster without changing the main loop.

•	Encapsulation:
o	Internal details such as health, poison flags, and room hazards are kept private to their classes and exposed only through controlled public methods, preserving invariants and simplifying reasoning. 
These features make it straightforward to extend the game, for example by adding a HealingRoom class that restores health, without modifying existing Game loop logic.

4.6 Error Handling and Robustness in Code
Input validation and boundary checks are implemented in Game and Map before any state changes are made.
Invalid moves, such as leaving the map or attempting to move more than the allowed distance, are rejected and reported to the player through UI messages rather than causing crashes.
Map operations such as getRoom are guarded by isInside checks, and any unexpected conditions can be detected during development using simple assertions. 
This defensive style supports the reliability and robustness requirements defined in Chapter 2 and helps maintain consistency between the implementation and the design models.
4.7 Testing and Validation
Testing focuses on verifying that the implementation behaves consistently with the use cases, sequence diagrams, and state diagrams defined earlier.
•	Unit-level checks:
o	Movement validation methods (isInside, adjacency checks).
o	Room effects (PoisonRoom and TrapRoom correctly update health, poison status, and remaining moves).
o	Player poison logic and death detection. 
•	Scenario-level tests:
o	Player successfully reaches the exit under normal conditions.
o	Player dies due to traps or poison.
o	Monster catches the player according to the designed movement strategy.
Observed behavior is compared with the expected flows from Figures 3.2–3.4 and the state transitions from Figures 3.7–3.8, thereby validating that the code correctly realizes the object-oriented design.  
