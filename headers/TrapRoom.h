#ifndef TRAPROOM_H
#define TRAPROOM_H

#include <string>
#include "Room.h"

class TrapRoom : public Room {
private:
    int damageAmount;

public:
    TrapRoom(const Position& pos, int dmg);
    std::string getType() const override;
    int getDamage() const;
    char getSymbol() const override;
    void visit(Player& player) override;
};

#endif
