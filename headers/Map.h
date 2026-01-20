#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Position.h"

class Room;

class Map {
private:
    int width;
    int height;
    Position startPos;
    Position exitPos;
    std::vector<std::vector<Room*>> rooms;

public:
    Map(int w, int h);
    ~Map();

    void initialize();  // create rooms, set start/exit and hazards

    bool isInside(const Position& pos) const;
    bool isAdjacent(const Position& from, const Position& to) const;

    Room* getRoom(const Position& pos) const;

    const Position& getStartPosition() const;
    const Position& getExitPosition() const;

    int getWidth() const;
    int getHeight() const;
};

#endif
