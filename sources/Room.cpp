#include "../headers/Room.h"

Room::Room(const Position& pos) : position(pos), visited(false) {}

const Position& Room::getPosition() const {
    return position;
}

bool Room::isVisited() const {
    return visited;
}

void Room::markVisited() {
    visited = true;
}
