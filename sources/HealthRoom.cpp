#include "../headers/HealthRoom.h"
#include "../headers/Player.h"
#include <iostream>

HealthRoom::HealthRoom(const Position& pos, int percentage)
    : Room(pos), healthPercentage(percentage) {
    setColor("blue");
    setLabel("Health");
}

std::string HealthRoom::getType() const {
    return "HealthRoom";
}

char HealthRoom::getSymbol() const {
    return visited ? 'H' : '.';  // Show as safe until discovered, then H for health
}

void HealthRoom::visit(Player& player) {
    int currentHealth = player.getHealth();
    bool wasPoisoned = player.isPoisoned();
    const int MAX_HEALTH = 100;
    
    // Calculate health to restore based on percentage
    int healthToRestore = getHealthRestored(MAX_HEALTH);
    
    // Restore health (capped at 100)
    player.heal(healthToRestore);
    int actualHealthRestored = player.getHealth() - currentHealth;
    
    // Show health restoration message
    std::cout << "\n*** HEALTH PICKUP! You restored " << actualHealthRestored 
              << " health (+" << healthPercentage << "% of max health)! ***\n";
    std::cout << "Current Health: " << player.getHealth() << "/" << MAX_HEALTH << "\n";
    
    // If player was poisoned, cure them
    if (wasPoisoned) {
        player.cure();
        std::cout << "*** YOU HAVE BEEN CURED! Poison removed! ***\n";
    }
    
    markVisited();
}

int HealthRoom::getHealthPercentage() const {
    return healthPercentage;
}

int HealthRoom::getHealthRestored(int maxHealth) const {
    return (maxHealth * healthPercentage) / 100;
}
