#include "../headers/Game.h"
#include "../headers/Room.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

Game::Game(int width, int height)
    : map(width, height),
      player(map.getStartPosition()),
      monster(map.getExitPosition()),  // or another start position
      gameOver(false),
      playerWon(false) {}

void Game::start() {
    map.initialize();
    player.setPosition(map.getStartPosition());
    monster.setPosition(map.getExitPosition());
    player.resetMoves();
    monster.resetMoves();
    gameOver = false;
    playerWon = false;
}

const Position& Game::getPlayerPosition() const {
    return player.getPosition();
}

const Position& Game::getMonsterPosition() const {
    return monster.getPosition();
}

const Position& Game::getExitPosition() const {
    return map.getExitPosition();
}

void Game::run() {
    while (!gameOver) {
        render();
        processPlayerTurn();
        if (gameOver) break;

        processMonsterTurn();
        if (gameOver) break;
    }
    render();
    showGameOver();
}

void Game::processPlayerTurn() {
    player.resetMoves();
    while (!gameOver && player.hasMovesLeft()) {
        std::cout << "\nMoves left: " << player.getRemainingMoves() << "\n";
        std::cout << "Enter move (W/A/S/D): ";
        char move;
        std::cin >> move;
        std::cin.ignore(1000, '\n');  // Clear input buffer

        Position current = player.getPosition();
        Position target = current;

        // Convert WASD to direction
        switch (move) {
            case 'w': case 'W':
                target.y++;
                break;
            case 's': case 'S':
                target.y--;
                break;
            case 'a': case 'A':
                target.x--;
                break;
            case 'd': case 'D':
                target.x++;
                break;
            default:
                std::cout << "Invalid input! Use W/A/S/D.\n";
                continue;
        }

        if (!map.isInside(target) || !map.isAdjacent(current, target)) {
            std::cout << "Invalid move - out of bounds or not adjacent!\n";
            continue;
        }

        Room* room = map.getRoom(target);
        bool wasUnvisited = !room->isVisited();
        player.move(target, map);
        room->visit(player);
        
        // Show message if entering a hazardous room for the first time
        if (wasUnvisited) {
            if (room->getSymbol() == 'T') {
                std::cout << "*** TRAP! You took 20 damage and your turn ended! ***\n";
            } else if (room->getSymbol() == 'P') {
                std::cout << "*** POISON! You've been poisoned! ***\n";
            }
        }
        
        player.update();

        // Show poison damage if poisoned
        if (player.isPoisoned()) {
            std::cout << "You suffer from poison damage!\n";
        }

        checkGameOver();
        if (gameOver || !player.hasMovesLeft()) break;

        render();
    }
}

void Game::processMonsterTurn() {
    monster.resetMoves();
    if (gameOver) return;

    Position target = monster.decideMove(*this);
    if (!map.isInside(target) || !map.isAdjacent(monster.getPosition(), target)) {
        return; // skip invalid move
    }

    monster.move(target, map);

    // check collision
    if (monster.getPosition().x == player.getPosition().x &&
        monster.getPosition().y == player.getPosition().y) {
        gameOver = true;
        playerWon = false;
    }
}

void Game::checkGameOver() {
    if (player.isDead()) {
        gameOver = true;
        playerWon = false;
    } else if (player.getPosition().x == map.getExitPosition().x &&
               player.getPosition().y == map.getExitPosition().y) {
        gameOver = true;
        playerWon = true;
    }
}

void Game::render() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    int w = map.getWidth();
    int h = map.getHeight();

    // Draw map
    for (int y = h - 1; y >= 0; --y) {
        for (int x = 0; x < w; ++x) {
            Position pos(x, y);

            if (player.getPosition().x == x && player.getPosition().y == y)
                std::cout << 'P';
            else if (monster.getPosition().x == x && monster.getPosition().y == y)
                std::cout << 'M';
            else {
                Room* room = map.getRoom(pos);
                std::cout << room->getSymbol();
            }
            std::cout << ' ';
        }
        std::cout << '\n';
    }

    // Display game info
    std::cout << "\n=== Game Status ===\n";
    std::cout << "Health: " << player.getHealth() << "/100";
    if (player.isPoisoned()) {
        std::cout << " [POISONED]";
    }
    std::cout << "\n";

    // Calculate distances
    int distToExit = player.getPosition().distanceTo(map.getExitPosition());
    int distToMonster = player.getPosition().distanceTo(monster.getPosition());
    
    std::cout << "Distance to Exit: " << distToExit << "\n";
    std::cout << "Distance to Monster: " << distToMonster << "\n";
    
    // Legend
    std::cout << "\nLegend: P=Player, M=Monster, .=Safe, ?=Unknown, T=Trap (visited), P=Poison (visited)\n";
}

void Game::showGameOver() const {
    std::cout << "\n=== GAME OVER ===\n";
    if (playerWon) {
        std::cout << "Congratulations! You reached the exit and escaped!\n";
    } else {
        if (player.isDead()) {
            std::cout << "You died! Your health reached zero.\n";
        } else {
            std::cout << "The monster caught you! Game over.\n";
        }
    }
    std::cout << "Final Health: " << player.getHealth() << "/100\n";
}
