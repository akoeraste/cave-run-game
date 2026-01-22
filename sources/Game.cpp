#include "../headers/Game.h"
#include "../headers/Room.h"
#include "../headers/TrapRoom.h"
#include "../headers/HealthRoom.h"
#include "../headers/Character.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

Game::Game(int rows, int cols)
    : map(rows, cols),
      player(map.getStartPosition()),
      monster(map.getExitPosition()),
      activeCharacter(&player),
      gameOver(false),
      playerWon(false),
      turnCount(0) {}

void Game::start() {
    map.initialize();
    player.setPosition(map.getStartPosition());
    monster.setPosition(map.getExitPosition());
    player.resetMoves();
    monster.resetMoves();
    activeCharacter = &player;
    gameOver = false;
    playerWon = false;
    turnCount = 0;
}

Position Game::getActivePosition() const {
    return activeCharacter ? activeCharacter->getPosition() : Position(0, 0);
}

bool Game::isGameOver() const {
    return gameOver;
}

bool Game::playerWins() const {
    return playerWon;
}

void Game::playerTurn() {
    player.resetMoves();
    while (!gameOver && player.hasMovesLeft()) {
        std::cout << "\nMoves left: " << player.getMoves() << "\n";
        std::cout << "Enter move (W/A/S/D): ";
        char move;
        std::cin >> move;
        std::cin.ignore(1000, '\n');  // Clear input buffer

        Position direction(0, 0);
        
        // Convert WASD to direction
        switch (move) {
            case 'w': case 'W':
                direction.y = 1;
                break;
            case 's': case 'S':
                direction.y = -1;
                break;
            case 'a': case 'A':
                direction.x = -1;
                break;
            case 'd': case 'D':
                direction.x = 1;
                break;
            default:
                std::cout << "Invalid input! Use W/A/S/D.\n";
                continue;
        }

        Position current = player.getPosition();
        Position target = current;
        target.x += direction.x;
        target.y += direction.y;

        if (!map.isValidPosition(target) || !map.isAdjacent(current, target)) {
            std::cout << "Invalid move - out of bounds or not adjacent!\n";
            continue;
        }

        player.setMoveDirection(direction);
        Room* room = map.getRoom(target);
        bool wasUnvisited = room && !room->isVisited();
        
        Position oldPos = player.getPosition();
        player.move(map);
        
        // Check if player actually moved and show messages
        if (player.getPosition().x != oldPos.x || player.getPosition().y != oldPos.y) {
            Room* currentRoom = map.getRoom(player.getPosition());
            
            // Show message if entering a special room for the first time
            // Note: room->visit() is called inside player.move(), so room is now visited
            // HealthRoom messages are handled inside visit() method
            if (currentRoom && wasUnvisited) {
                if (currentRoom->getType() == "TrapRoom") {
                    std::cout << "*** TRAP! You took " << dynamic_cast<TrapRoom*>(currentRoom)->getDamage() 
                              << " damage and your turn ended! ***\n";
                } else if (currentRoom->getType() == "PoisonRoom") {
                    std::cout << "*** POISON! You've been poisoned! ***\n";
                }
                // HealthRoom messages are displayed in visit() method
            }
        }
        
        player.update();

        // Show poison damage if poisoned
        if (player.isPoisoned()) {
            std::cout << "You suffer from poison damage!\n";
        }

        checkGameOver();
        if (gameOver || !player.hasMovesLeft()) break;

        displayMap();
    }
}

void Game::monsterTurn() {
    monster.resetMoves();
    if (gameOver) return;

    monster.sense(*this);
    monster.move(map);

    // check collision
    if (monster.getPosition().x == player.getPosition().x &&
        monster.getPosition().y == player.getPosition().y) {
        gameOver = true;
        playerWon = false;
    }
}

void Game::switchTurn() {
    if (activeCharacter == &player) {
        activeCharacter = &monster;
    } else {
        activeCharacter = &player;
    }
    turnCount++;
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

void Game::run() {
    while (!gameOver) {
        displayMap();
        playerTurn();
        if (gameOver) break;

        monsterTurn();
        if (gameOver) break;
        
        switchTurn();
    }
    displayMap();
    displayGameOver();
}

// Legacy method names for backward compatibility
void Game::processPlayerTurn() {
    playerTurn();
}

void Game::processMonsterTurn() {
    monsterTurn();
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

void Game::displayMap() const {
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
                std::cout << 'A';
            else if (monster.getPosition().x == x && monster.getPosition().y == y)
                std::cout << 'M';
            else {
                Room* room = map.getRoom(pos);
                if (room) {
                    std::cout << room->getSymbol();
                } else {
                    std::cout << '?';
                }
            }
            std::cout << ' ';
        }
        std::cout << '\n';
    }

    // Display game info
    std::cout << "\n=== Game Status ===\n";
    std::cout << "Turn: " << turnCount << "\n";
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
    std::cout << "\nLegend: A=Player, M=Monster, .=Unvisited/Safe, T=Trap (discovered), P=Poison (discovered), H=Health (discovered)\n";
}

void Game::displayGameOver() const {
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
    std::cout << "Total Turns: " << turnCount << "\n";
}

// Legacy method names for backward compatibility
void Game::render() const {
    displayMap();
}

void Game::showGameOver() const {
    displayGameOver();
}
