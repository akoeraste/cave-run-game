#include "../headers/Monster.h"
#include "../headers/Game.h"
#include "../headers/Map.h"

Monster::Monster(const Position& startPos)
    : Character(startPos, 1) {}   // 1 move per turn

void Monster::sense(const Game& /*game*/) {
    // Monster uses info via decideMove, no internal state needed here
}

Position Monster::decideMove(const Game& game) {
    Position playerPos = game.getPlayerPosition();
    Position exitPos   = game.getExitPosition();

    int monsterToExit = position.distanceTo(exitPos);
    int playerToExit  = playerPos.distanceTo(exitPos);

    Position target = position;

    // If monster is closer to exit than player, move towards player; else towards exit
    Position goal = (monsterToExit < playerToExit) ? playerPos : exitPos;

    // Move one step at a time (prefer horizontal, then vertical)
    int dx = goal.x - position.x;
    int dy = goal.y - position.y;

    if (dx != 0) {
        // Move horizontally first
        target.x += (dx > 0) ? 1 : -1;
    } else if (dy != 0) {
        // Then move vertically
        target.y += (dy > 0) ? 1 : -1;
    }

    return target;
}
