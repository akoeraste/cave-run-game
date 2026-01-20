#include "../headers/Position.h"
#include <cmath>

Position::Position(int x, int y) : x(x), y(y) {}

int Position::distanceTo(const Position& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::abs(dx) + std::abs(dy);  // Manhattan distance is fine
}

bool Position::isAdjacentTo(const Position& other) const {
    int dx = std::abs(x - other.x);
    int dy = std::abs(y - other.y);
    return (dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0));
}
