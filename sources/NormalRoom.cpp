#include "../headers/NormalRoom.h"
#include "../headers/Player.h"

NormalRoom::NormalRoom(const Position& pos) : Room(pos) {
    setColor("white");
    setLabel("Normal");
}

std::string NormalRoom::getType() const {
    return "NormalRoom";
}

char NormalRoom::getSymbol() const {
    return '.';   // safe room
}

void NormalRoom::visit(Player& player) {
    // No special effect
    markVisited();
}
