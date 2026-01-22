#ifndef MONSTER_H
#define MONSTER_H

#include "Character.h"

class Monster : public Character {
private:
    Position playerPosition;
    Position exitPosition;

public:
    Monster(const Position& startPos);

    void sense(const Game& game) override;
    Position getDirection() override;
    void move(const Map& map) override;
    void update() override;
    
    double calculateDistance(const Position& p1, const Position& p2) const;
};

#endif
