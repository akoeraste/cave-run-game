#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "HealthStatus.h"

class Player : public Character {
private:
    HealthStatus healthStatus;
    int poisonDamage;
    double cureChance;
    Position moveDirection;

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
    
    void sense(const Game& game) override;
    void move(const Map& map) override;
    void update() override;
};

#endif
