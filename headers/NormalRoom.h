#ifndef NORMALROOM_H
#define NORMALROOM_H

#include <string>
#include "Room.h"

class NormalRoom : public Room {
public:
    NormalRoom(const Position& pos);
    std::string getType() const override;
    char getSymbol() const override;
    void visit(Player& player) override;
};

#endif
