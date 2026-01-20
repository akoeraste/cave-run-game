#include "../headers/TrapRoom.h"
#include "../headers/Player.h"

TrapRoom::TrapRoom(const Position& pos, int dmg)
    : Room(pos), damageAmount(dmg) {}

char TrapRoom::getSymbol() const {
    return visited ? 'T' : '?';  // T for trap (red) after discovery
}

void TrapRoom::visit(Player& player) {
    player.damage(damageAmount);
    player.setRemainingMoves(0);   // end turn
    markVisited();
}
