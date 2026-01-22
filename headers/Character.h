#ifndef CHARACTER_H
#define CHARACTER_H

#include "Position.h"

class Game;
class Map;
class Room;

class Character {
protected:
    Position position;
    Room* currentRoom;
    int movesRemaining;
    int maxMoves;

public:
    Character(const Position& startPos, int movesPerTurn);
    virtual ~Character() = default;

    const Position& getPosition() const;
    void setPosition(const Position& pos);
    
    void setRoom(Room* room);
    Room* getRoom() const;

    void setMoves(int m);
    int getMoves() const;
    void resetMoves();
    bool hasMovesLeft() const;

    virtual void sense(const Game& game) = 0;
    virtual Position getDirection() = 0;
    virtual void move(const Map& map) = 0;
    virtual void update() {}  // default no-op
};

#endif
