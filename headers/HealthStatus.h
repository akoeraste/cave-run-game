#ifndef HEALTHSTATUS_H
#define HEALTHSTATUS_H

class HealthStatus {
private:
    int health;
    bool poisoned;

public:
    HealthStatus(int initialHealth = 100);
    
    void damage(int amount);
    void heal(int amount);
    bool isDead() const;
    bool isPoisoned() const;
    void setPoisoned(bool flag);
    int getHealth() const;
};

#endif
