#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"

class Monster : public Character {
public:
    Monster(const Position& startPos);

    void sense(const Game& game) override;
    Position decideMove(const Game& game) override;
};

#endif
