#ifndef TRAPROOM_H
#define TRAPROOM_H

#include "Room.h"

class TrapRoom : public Room {
private:
    int damageAmount;

public:
    TrapRoom(const Position& pos, int dmg);
    char getSymbol() const override;
    void visit(Player& player) override;
};

#endif
