#ifndef POISONROOM_H
#define POISONROOM_H

#include <string>
#include "Room.h"

class PoisonRoom : public Room {
private:
    int poisonDamage;

public:
    PoisonRoom(const Position& pos, int dmg);
    std::string getType() const override;
    char getSymbol() const override;
    void visit(Player& player) override;
};

#endif
