#include "../headers/Room.h"

Room::Room(const Position& pos) : position(pos), color(""), label(""), visited(false) {}

const Position& Room::getPosition() const {
    return position;
}

void Room::setColor(const std::string& c) {
    color = c;
}

std::string Room::getColor() const {
    return color;
}

void Room::setLabel(const std::string& l) {
    label = l;
}

std::string Room::getLabel() const {
    return label;
}

bool Room::isVisited() const {
    return visited;
}

void Room::markVisited() {
    visited = true;
}
