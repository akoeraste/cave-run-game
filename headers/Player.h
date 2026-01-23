#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include <string>

class Player : public Character {
private:
    int health;
    bool poisoned;
    int poisonDamage;
    double cureChance;
    Position moveDirection;
    mutable std::string uiMessage; // one-shot message shown near Health (mutable for const access)

public:
    Player(const Position& startPos, int initialHealth = 100);

    int getHealth() const;
    void damage(int amount);
    void heal(int amount);
    bool isAlive() const;
    bool isDead() const;
    
    void setPoisoned(bool flag);
    bool isPoisoned() const;
    void setPoisonDamage(int dmg);
    void cure();

    void setMoveDirection(const Position& dir);
    Position getDirection() override;

    // UI helpers (for displaying messages in Game::displayMap)
    void setUiMessage(const std::string& message);
    std::string consumeUiMessage() const;
    
    void sense(const Game& game) override;
    void move(const Map& map) override;
    void update() override;
};

#endif
