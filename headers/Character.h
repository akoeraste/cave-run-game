#ifndef CHARACTER_H
#define CHARACTER_H

#include "Position.h"

class Game;
class Map;

class Character {
protected:
    Position position;
    int remainingMoves;
    int maxMovesPerTurn;

public:
    Character(const Position& startPos, int movesPerTurn);
    virtual ~Character() = default;

    const Position& getPosition() const;
    void setPosition(const Position& pos);

    void resetMoves();
    bool hasMovesLeft() const;
    int getRemainingMoves() const;
    void setRemainingMoves(int moves);

    virtual void sense(const Game& game) = 0;
    virtual Position decideMove(const Game& game) = 0;
    virtual void update() {}  // default no-op

    void move(const Position& target, const Map& map);
};

#endif
