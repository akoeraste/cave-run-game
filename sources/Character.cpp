#include "../headers/Character.h"
#include "../headers/Map.h"
#include "../headers/Room.h"

Character::Character(const Position& startPos, int movesPerTurn)
    : position(startPos), currentRoom(nullptr), movesRemaining(movesPerTurn), maxMoves(movesPerTurn) {}

const Position& Character::getPosition() const {
    return position;
}

void Character::setPosition(const Position& pos) {
    position = pos;
}

void Character::setRoom(Room* room) {
    currentRoom = room;
}

Room* Character::getRoom() const {
    return currentRoom;
}

void Character::setMoves(int m) {
    movesRemaining = m;
}

int Character::getMoves() const {
    return movesRemaining;
}

void Character::resetMoves() {
    movesRemaining = maxMoves;
}

bool Character::hasMovesLeft() const {
    return movesRemaining > 0;
}
