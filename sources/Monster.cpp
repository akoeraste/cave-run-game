#include "../headers/Monster.h"
#include "../headers/Game.h"
#include "../headers/Map.h"
#include <cmath>

Monster::Monster(const Position& startPos)
    : Character(startPos, 1),   // 1 move per turn
      playerPosition(0, 0),
      exitPosition(0, 0) {}

void Monster::sense(const Game& game) {
    playerPosition = game.getPlayerPosition();
    exitPosition = game.getExitPosition();
}

Position Monster::getDirection() {
    double monsterToExit = calculateDistance(position, exitPosition);
    double playerToExit = calculateDistance(playerPosition, exitPosition);

    Position goal = (monsterToExit < playerToExit) ? playerPosition : exitPosition;

    // Calculate direction (one step at a time, prefer horizontal, then vertical)
    Position direction(0, 0);
    int dx = goal.x - position.x;
    int dy = goal.y - position.y;

    if (dx != 0) {
        // Move horizontally first
        direction.x = (dx > 0) ? 1 : -1;
    } else if (dy != 0) {
        // Then move vertically
        direction.y = (dy > 0) ? 1 : -1;
    }

    return direction;
}

void Monster::move(const Map& map) {
    if (!hasMovesLeft()) return;
    
    Position direction = getDirection();
    Position target = position;
    target.x += direction.x;
    target.y += direction.y;
    
    // Validate move
    if (map.isInside(target) && map.isAdjacent(position, target)) {
        position = target;
        movesRemaining--;
        
        // Update current room
        Room* room = map.getRoom(target);
        if (room) {
            setRoom(room);
        }
    }
}

void Monster::update() {
    // Monster doesn't need per-turn updates
}

double Monster::calculateDistance(const Position& p1, const Position& p2) const {
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);  // Euclidean distance
}
