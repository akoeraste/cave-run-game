#include "../headers/HealthStatus.h"

HealthStatus::HealthStatus(int initialHealth)
    : health(initialHealth), poisoned(false) {}

void HealthStatus::damage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void HealthStatus::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;  // Cap at 100
}

bool HealthStatus::isDead() const {
    return health <= 0;
}

bool HealthStatus::isPoisoned() const {
    return poisoned;
}

void HealthStatus::setPoisoned(bool flag) {
    poisoned = flag;
}

int HealthStatus::getHealth() const {
    return health;
}
