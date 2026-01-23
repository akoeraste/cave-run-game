#include "headers/Game.h"
#include <iostream>

int main() {
    char choice;
    
    do {
        Game game(8, 8);   // 8 rows x 8 cols map
        game.start();
        game.run();
        
        std::cout << "\nWould you like to play again? (Y/N): ";
        std::cin >> choice;
        std::cin.ignore(1000, '\n');  // Clear input buffer
        
    } while (choice == 'Y' || choice == 'y');
    
    std::cout << "Thanks for playing! Goodbye!\n";
    return 0;
}
