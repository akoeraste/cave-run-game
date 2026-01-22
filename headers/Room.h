#ifndef ROOM_H
#define ROOM_H

#include <string>
#include "Position.h"

class Player;  // forward declaration

class Room {
protected:
    Position position;
    std::string color;
    std::string label;
    bool visited;

public:
    Room(const Position& pos);
    virtual ~Room() = default;

    const Position& getPosition() const;
    void setColor(const std::string& c);
    std::string getColor() const;
    void setLabel(const std::string& l);
    std::string getLabel() const;
    bool isVisited() const;
    void markVisited();
    
    virtual std::string getType() const = 0;
    virtual char getSymbol() const = 0;          // for console display
    virtual void visit(Player& player) = 0;      // polymorphic effect
};

#endif
