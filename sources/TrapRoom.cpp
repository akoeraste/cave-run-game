#include "../headers/TrapRoom.h"
#include "../headers/Player.h"
#include <iostream>

TrapRoom::TrapRoom(const Position& pos, int dmg)
    : Room(pos), damageAmount(dmg) {
    setColor("red");
    setLabel("Trap");
}

std::string TrapRoom::getType() const {
    return "TrapRoom";
}

int TrapRoom::getDamage() const {
    return damageAmount;
}

char TrapRoom::getSymbol() const {
    return visited ? 'T' : '?';  // Show as ? until discovered, then T for trap
}

void TrapRoom::visit(Player& player) {
    if (!visited) {
        int healthBefore = player.getHealth();
        player.damage(damageAmount);
        int healthAfter = player.getHealth();
        player.setMoves(0);   // end turn
        
        std::cout << "It's a trap! -" << damageAmount << " HP (Health: " << healthBefore 
                  << "/100 -> " << healthAfter << "/100)\n";
    }
    markVisited();
}
