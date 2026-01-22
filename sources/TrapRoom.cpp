#include "../headers/TrapRoom.h"
#include "../headers/Player.h"

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
    return visited ? 'T' : '.';  // Show as safe until discovered, then T for trap
}

void TrapRoom::visit(Player& player) {
    player.damage(damageAmount);
    player.setMoves(0);   // end turn
    markVisited();
}
