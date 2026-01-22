#include "../headers/PoisonRoom.h"
#include "../headers/Player.h"

PoisonRoom::PoisonRoom(const Position& pos, int dmg)
    : Room(pos), poisonDamage(dmg) {
    setColor("green");
    setLabel("Poison");
}

std::string PoisonRoom::getType() const {
    return "PoisonRoom";
}

char PoisonRoom::getSymbol() const {
    return visited ? 'P' : '.';  // Show as safe until discovered, then P for poison
}

void PoisonRoom::visit(Player& player) {
    player.setPoisoned(true);
    player.setPoisonDamage(poisonDamage);
    markVisited();
}
