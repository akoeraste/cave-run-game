#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Position.h"

class Room;

class Map {
private:
    std::vector<std::vector<Room*>> rooms;
    int rows;
    int cols;
    Position exitPosition;

public:
    Map(int rows, int cols);
    ~Map();

    void initialize();  // create rooms, set start/exit and hazards
    void addRoom(Room* room);

    Room* getRoom(const Position& pos) const;
    Position getDimensions() const;
    const Position& getExitPosition() const;
    void setExitPosition(const Position& pos);
    bool isValidPosition(const Position& pos) const;
    bool isInside(const Position& pos) const;  // kept for backward compatibility
    bool isAdjacent(const Position& from, const Position& to) const;
    void display() const;
    
    // Legacy methods for backward compatibility
    const Position& getStartPosition() const;
    int getWidth() const;
    int getHeight() const;
};

#endif
