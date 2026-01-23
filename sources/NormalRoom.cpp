#include "../headers/NormalRoom.h"
#include "../headers/Player.h"
#include <iostream>

NormalRoom::NormalRoom(const Position& pos, bool hasPowerup, int healthPercent)
    : Room(pos), hasHealthPowerup(hasPowerup), healthPercentage(healthPercent) {
    setColor("white");
    setLabel("Normal");
}

std::string NormalRoom::getType() const {
    return "NormalRoom";
}

char NormalRoom::getSymbol() const {
    return '.';   // safe room (always shows as .)
}

void NormalRoom::visit(Player& player) {
    if (hasHealthPowerup && !visited) {
        // Restore health (20% of max health, capped at 100)
        const int MAX_HEALTH = 100;
        int healthToRestore = (MAX_HEALTH * healthPercentage) / 100;
        int currentHealth = player.getHealth();
        bool wasPoisoned = player.isPoisoned();
        
        player.heal(healthToRestore);
        int actualHealthRestored = player.getHealth() - currentHealth;
        
        // Show health restoration message
        std::cout << "Health pickup! +" << actualHealthRestored << " HP (Health: " << currentHealth 
                  << "/100 -> " << player.getHealth() << "/100)\n";
        
        // If player was poisoned, cure them
        if (wasPoisoned) {
            player.cure();
            std::cout << "You have been cured! Poison removed.\n";
        }
    }
    
    markVisited();
}

bool NormalRoom::hasPowerup() const {
    return hasHealthPowerup;
}
