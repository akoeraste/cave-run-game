#include "../headers/Player.h"
#include <iostream>
#include <cstdlib>

Player::Player(const Position& startPos, int initialHealth)
    : Character(startPos, 2),   // 2 moves per turn
      health(initialHealth),
      poisoned(false),
      poisonDamage(0),
      cureProbability(0.2) {}   // 20% cure chance

void Player::damage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool Player::isDead() const {
    return health <= 0;
}

int Player::getHealth() const {
    return health;
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

void Player::sense(const Game& /*game*/) {
    // For console version, player senses mainly via the UI display
}

Position Player::decideMove(const Game& /*game*/) {
    // In console version, Game will read input and compute target,
    // so this may not be used heavily; keep simple or unused.
    return position;
}

void Player::update() {
    if (poisoned && !isDead()) {
        damage(poisonDamage);
        // simple cure chance
        double r = (double) std::rand() / RAND_MAX;
        if (r < cureProbability) {
            poisoned = false;
        }
    }
}
