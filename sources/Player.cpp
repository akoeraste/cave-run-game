#include "../headers/Player.h"
#include "../headers/Game.h"
#include "../headers/Map.h"
#include "../headers/Room.h"
#include <iostream>
#include <cstdlib>

Player::Player(const Position& startPos, int initialHealth)
    : Character(startPos, 2),   // 2 moves per turn
      health(initialHealth),
      poisoned(false),
      poisonDamage(0),
      cureChance(0.2),   // 20% cure chance
      moveDirection(0, 0) {}

int Player::getHealth() const {
    return health;
}

void Player::damage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Player::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;  // Cap at 100
}

bool Player::isAlive() const {
    return health > 0;
}

bool Player::isDead() const {
    return health <= 0;
}

void Player::setPoisoned(bool flag) {
    poisoned = flag;
}

bool Player::isPoisoned() const {
    return poisoned;
}

void Player::setPoisonDamage(int dmg) {
    poisonDamage = dmg;
}

void Player::cure() {
    poisoned = false;
}

void Player::setMoveDirection(const Position& dir) {
    moveDirection = dir;
}

Position Player::getDirection() {
    return moveDirection;
}

void Player::sense(const Game& /*game*/) {
    // For console version, player senses mainly via the UI display
}

void Player::move(const Map& map) {
    if (!hasMovesLeft()) return;
    
    Position target = position;
    target.x += moveDirection.x;
    target.y += moveDirection.y;
    
    // Validate move
    if (map.isInside(target) && map.isAdjacent(position, target)) {
        position = target;
        movesRemaining--;
        
        // Update current room
        Room* room = map.getRoom(target);
        if (room) {
            setRoom(room);
            room->visit(*this);
        }
    }
}

void Player::update() {
    if (poisoned && isAlive()) {
        damage(poisonDamage);
        // simple cure chance
        double r = (double) std::rand() / RAND_MAX;
        if (r < cureChance) {
            cure();
        }
    }
}
