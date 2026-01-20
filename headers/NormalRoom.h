#ifndef NORMALROOM_H
#define NORMALROOM_H

#include "Room.h"

class NormalRoom : public Room {
public:
    NormalRoom(const Position& pos);
    char getSymbol() const override;
    void visit(Player& player) override;
};

#endif
