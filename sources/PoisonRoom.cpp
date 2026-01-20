#include "../headers/PoisonRoom.h"
#include "../headers/Player.h"

PoisonRoom::PoisonRoom(const Position& pos, int dmg)
    : Room(pos), poisonDamage(dmg) {}

char PoisonRoom::getSymbol() const {
    return visited ? 'P' : '?';  // P for poison (green) after discovery
}

void PoisonRoom::visit(Player& player) {
    player.setPoisoned(true);
    player.setPoisonDamage(poisonDamage);
    markVisited();
}
