#ifndef ROOM_H
#define ROOM_H

#include "Position.h"

class Player;  // forward declaration

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

    virtual char getSymbol() const = 0;          // for console display
    virtual void visit(Player& player) = 0;      // polymorphic effect
};

#endif
