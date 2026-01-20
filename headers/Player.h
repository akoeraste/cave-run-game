#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"

class Player : public Character {
private:
    int health;
    bool poisoned;
    int poisonDamage;
    double cureProbability;

public:
    Player(const Position& startPos, int initialHealth = 100);

    void damage(int amount);
    bool isDead() const;
    int getHealth() const;

    void setPoisoned(bool flag);
    bool isPoisoned() const;
    void setPoisonDamage(int dmg);

    void sense(const Game& game) override;
    Position decideMove(const Game& game) override;
    void update() override;
};

#endif
