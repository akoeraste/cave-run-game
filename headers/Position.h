#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int x;
    int y;

    Position(int x = 0, int y = 0);

    int distanceTo(const Position& other) const;
    bool isAdjacentTo(const Position& other) const;
};

#endif
