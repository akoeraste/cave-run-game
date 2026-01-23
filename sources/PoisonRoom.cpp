#include "../headers/PoisonRoom.h"
#include "../headers/Player.h"
#include <iostream>

PoisonRoom::PoisonRoom(const Position& pos, int dmg)
    : Room(pos), poisonDamage(dmg) {
    setColor("green");
    setLabel("Poison");
}

std::string PoisonRoom::getType() const {
    return "PoisonRoom";
}

char PoisonRoom::getSymbol() const {
    return visited ? 'P' : '?';  // Show as ? until discovered, then P for poison
}

void PoisonRoom::visit(Player& player) {
    if (!visited) {
        int healthBefore = player.getHealth();
        player.setPoisoned(true);
        player.setPoisonDamage(poisonDamage);
        
        std::cout << "You've been poisoned! (Health: " << healthBefore << "/100 -> " 
                  << player.getHealth() << "/100)\n";
    }
    markVisited();
}
