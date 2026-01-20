#include "../headers/Character.h"
#include "../headers/Map.h"

Character::Character(const Position& startPos, int movesPerTurn)
    : position(startPos), remainingMoves(movesPerTurn), maxMovesPerTurn(movesPerTurn) {}

const Position& Character::getPosition() const {
    return position;
}

void Character::setPosition(const Position& pos) {
    position = pos;
}

void Character::resetMoves() {
    remainingMoves = maxMovesPerTurn;
}

bool Character::hasMovesLeft() const {
    return remainingMoves > 0;
}

int Character::getRemainingMoves() const {
    return remainingMoves;
}

void Character::setRemainingMoves(int moves) {
    remainingMoves = moves;
}

void Character::move(const Position& target, const Map& map) {
    // Map bounds / adjacency should already be checked by Game
    position = target;
    remainingMoves--;
}
