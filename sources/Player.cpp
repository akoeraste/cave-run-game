#include "../headers/Player.h"
#include "../headers/Game.h"
#include "../headers/Map.h"
#include <iostream>
#include <cstdlib>

Player::Player(const Position& startPos, int initialHealth)
    : Character(startPos, 2),   // 2 moves per turn
      healthStatus(initialHealth),
      poisonDamage(0),
      cureChance(0.2),   // 20% cure chance
      moveDirection(0, 0) {}

int Player::getHealth() const {
    return healthStatus.getHealth();
}

void Player::damage(int amount) {
    healthStatus.damage(amount);
}

void Player::heal(int amount) {
    healthStatus.heal(amount);
}

bool Player::isAlive() const {
    return !healthStatus.isDead();
}

bool Player::isDead() const {
    return healthStatus.isDead();
}

void Player::setPoisoned(bool flag) {
    healthStatus.setPoisoned(flag);
}

bool Player::isPoisoned() const {
    return healthStatus.isPoisoned();
}

void Player::setPoisonDamage(int dmg) {
    poisonDamage = dmg;
}

void Player::cure() {
    healthStatus.setPoisoned(false);
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
    if (healthStatus.isPoisoned() && isAlive()) {
        healthStatus.damage(poisonDamage);
        // simple cure chance
        double r = (double) std::rand() / RAND_MAX;
        if (r < cureChance) {
            cure();
        }
    }
}
