#ifndef POISONROOM_H
#define POISONROOM_H

#include "Room.h"

class PoisonRoom : public Room {
private:
    int poisonDamage;

public:
    PoisonRoom(const Position& pos, int dmg);
    char getSymbol() const override;
    void visit(Player& player) override;
};

#endif
