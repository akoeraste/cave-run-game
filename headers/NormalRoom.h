#ifndef NORMALROOM_H
#define NORMALROOM_H

#include <string>
#include "Room.h"

class NormalRoom : public Room {
private:
    bool hasHealthPowerup;
    int healthPercentage;  // Percentage of max health to restore (e.g., 20)

public:
    NormalRoom(const Position& pos, bool hasPowerup = false, int healthPercent = 20);
    std::string getType() const override;
    char getSymbol() const override;
    void visit(Player& player) override;
    bool hasPowerup() const;
};

#endif
