#include "headers/Game.h"

int main() {
    Game game(8, 8);   // 8 rows x 8 cols map
    game.start();
    game.run();
    return 0;
}
