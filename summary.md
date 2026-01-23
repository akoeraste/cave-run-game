# Chapter 4 — Object-Oriented Programming (OOP) Summary (Cave Run)

This chapter summarizes how the **Cave Run** C++ codebase applies the core OOP principles:

- **Inheritance**
- **Polymorphism**
- **Encapsulation**
- **Abstraction**

The examples below use short **snippets from the actual implementation** to show where each concept appears.

---

## 4.1 Inheritance (is-a relationships)

Inheritance is used to model natural “is-a” relationships in the game:

- A `Player` **is a** `Character`
- A `Monster` **is a** `Character`
- A `NormalRoom`, `PoisonRoom`, and `TrapRoom` **are** `Room`s

### Character → Player / Monster

`Player` inherits shared movement/position state from `Character`:

```cpp
class Player : public Character {
private:
    int health;
    bool poisoned;
    // ...
public:
    Player(const Position& startPos, int initialHealth = 100);
    // ...
    Position getDirection() override;
    void move(const Map& map) override;
    void update() override;
};
```

`Monster` also inherits from `Character` and specializes sensing/decision logic:

```cpp
class Monster : public Character {
private:
    Position playerPosition;
    Position exitPosition;
public:
    Monster(const Position& startPos);
    void sense(const Game& game) override;
    Position getDirection() override;
    void move(const Map& map) override;
};
```

### Room → NormalRoom / PoisonRoom / TrapRoom

The room hierarchy models different room behaviors without changing the game loop.

---

## 4.2 Abstraction (interfaces via abstract base classes)

Abstraction is achieved using **abstract base classes** with **pure virtual functions**.
These define *what* must be done, without enforcing *how* each subclass does it.

### Abstract Character

`Character` is an abstract base type that defines a common contract for all moving entities:

```cpp
class Character {
protected:
    Position position;
    Room* currentRoom;
    int movesRemaining;
    int maxMoves;

public:
    virtual void sense(const Game& game) = 0;
    virtual Position getDirection() = 0;
    virtual void move(const Map& map) = 0;
    virtual void update() {}  // default no-op
};
```

This allows the game to treat `Player` and `Monster` uniformly as “things that can take turns”.

### Abstract Room

`Room` is an abstract base type defining a common interface for room types:

```cpp
class Room {
protected:
    Position position;
    bool visited;
public:
    virtual std::string getType() const = 0;
    virtual char getSymbol() const = 0;
    virtual void visit(Player& player) = 0;
};
```

This means new room types can be added by implementing `visit()` and `getSymbol()` without rewriting the turn logic.

---

## 4.3 Polymorphism (same call, different behavior)

Polymorphism occurs when the program calls a virtual function through a **base-class pointer/reference**,
and the **derived implementation** runs at runtime.

### Polymorphic room effects: `Room*` + `visit(player)`

Rooms are stored and retrieved as `Room*` in the `Map` grid:

```cpp
std::vector<std::vector<Room*>> rooms;
```

When the player moves, the game calls:

```cpp
Room* room = map.getRoom(target);
if (room) {
    room->visit(*this);
}
```

The call `room->visit(player)` executes the correct version automatically:

- `NormalRoom::visit()` might grant a health powerup and/or cure poison
- `PoisonRoom::visit()` sets poison status and poison damage
- `TrapRoom::visit()` applies damage and ends the player’s turn

Example: `TrapRoom::visit()`:

```cpp
void TrapRoom::visit(Player& player) {
    if (!visited) {
        player.damage(damageAmount);
        player.setMoves(0);   // end turn
    }
    markVisited();
}
```

Example: `PoisonRoom::visit()`:

```cpp
void PoisonRoom::visit(Player& player) {
    if (!visited) {
        player.setPoisoned(true);
        player.setPoisonDamage(poisonDamage);
    }
    markVisited();
}
```

### Polymorphic characters: Player vs Monster

Both `Player` and `Monster` implement their own `getDirection()`/`move()` logic.
For example, the monster computes its movement based on distances to the exit and player:

```cpp
Position Monster::getDirection() {
    double monsterToExit = calculateDistance(position, exitPosition);
    double playerToExit  = calculateDistance(playerPosition, exitPosition);
    Position goal = (monsterToExit < playerToExit) ? playerPosition : exitPosition;
    // ... choose one-step direction ...
    return direction;
}
```

---

## 4.4 Encapsulation (data hiding + controlled access)

Encapsulation is shown by keeping data **private/protected** and exposing **public methods** to interact with it safely.

### Encapsulation in Player health/poison

Player state is private:

```cpp
class Player : public Character {
private:
    int health;
    bool poisoned;
    int poisonDamage;
    double cureChance;
    // ...
};
```

And is modified only through methods like:

```cpp
void Player::damage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;
}
```

This prevents invalid states like negative health or health exceeding 100.

### Encapsulation in movement rules

Movement and turn rules are encapsulated through:

- `movesRemaining` tracked in `Character`
- `resetMoves()`, `setMoves()`, `hasMovesLeft()` controlling how turns progress

```cpp
void Character::resetMoves() {
    movesRemaining = maxMoves;
}

bool Character::hasMovesLeft() const {
    return movesRemaining > 0;
}
```

Rooms can end the player’s turn *without directly touching internal variables* by calling `player.setMoves(0)`.

---

## 4.5 Why this design is extendable

This OOP structure makes the code easy to extend:

- Add a new room type by subclassing `Room` and overriding `visit()`/`getSymbol()`
- Add new character types by subclassing `Character` and overriding `sense()`/`getDirection()`/`move()`
- Keep the main loop stable because it relies on base-class interfaces, not concrete implementations

---

## 4.6 Conclusion

The Cave Run implementation applies OOP consistently:

- **Inheritance** structures the domain (characters and rooms).
- **Abstraction** defines stable interfaces (`Character`, `Room`).
- **Polymorphism** enables flexible runtime behavior (room effects, AI behavior) without type checks.
- **Encapsulation** protects state (health, poison, movement rules) and enforces valid game behavior.

