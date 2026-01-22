#ifndef HEALTHROOM_H
#define HEALTHROOM_H

#include <string>
#include "Room.h"

class HealthRoom : public Room {
private:
    int healthPercentage;  // Percentage of max health (e.g., 10, 15, 20)

public:
    HealthRoom(const Position& pos, int percentage);
    std::string getType() const override;
    char getSymbol() const override;
    void visit(Player& player) override;
    int getHealthPercentage() const;
    int getHealthRestored(int maxHealth) const;  // Calculate actual health restored
};

#endif
