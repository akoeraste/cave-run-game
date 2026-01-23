#include "../headers/Game.h"
#include "../headers/Room.h"
#include "../headers/TrapRoom.h"
#include "../headers/Character.h"
#include <iostream>
#include <thread>
#include <chrono>

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
    player.cure();  // Clear poison status and any status flags
    player.setPoisonDamage(0);  // Reset poison damage
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
    while (!gameOver && player.hasMovesLeft()) {
        std::cout << "Enter your move (W/A/S/D): ";
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
            // Health powerup messages are handled inside NormalRoom::visit() method
            if (currentRoom && wasUnvisited) {
                if (currentRoom->getType() == "TrapRoom") {
                    std::cout << "It's a trap! -" << dynamic_cast<TrapRoom*>(currentRoom)->getDamage() 
                              << " HP\n";
                } else if (currentRoom->getType() == "PoisonRoom") {
                    std::cout << "You've been poisoned!\n";
                }
                // Health powerup messages are displayed in NormalRoom::visit() method
            }
        }
        
        player.update();

        // Show poison damage if poisoned
        if (player.isPoisoned() && !gameOver) {
            int healthAfterPoison = player.getHealth();
            std::cout << "Poison effect: -5 HP (Health: " << healthAfterPoison << "/100)\n";
        }

        checkGameOver();
        if (gameOver || !player.hasMovesLeft()) break;

        displayMap();
    }
}

void Game::monsterTurn() {
    monster.resetMoves();
    if (gameOver) return;

    std::cout << "\nMonster is thinking...\n";
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::seconds(2));

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
        // Ensure UI shows fresh moves at the start of the player's turn.
        player.resetMoves();
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

    // Game Title
    std::cout << "\n";
    std::cout << "=== CAVE RUN GAME ===\n";
    std::cout << "\n";

    int w = map.getWidth();
    int h = map.getHeight();

    // Draw map with coordinates
    std::cout << "    ";
    for (int x = 0; x < w; ++x) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    for (int y = h - 1; y >= 0; --y) {
        std::cout << y << "  ";
        for (int x = 0; x < w; ++x) {
            Position pos(x, y);

            if (player.getPosition().x == x && player.getPosition().y == y)
                std::cout << 'A';
            else if (monster.getPosition().x == x && monster.getPosition().y == y)
                std::cout << 'M';
            else if (pos.x == map.getExitPosition().x && pos.y == map.getExitPosition().y)
                std::cout << 'E';
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
        std::cout << "\n";
    }

    std::cout << "\n";

    // Legend
    std::cout << "Legend:\n";
    std::cout << "A = Player\n";
    std::cout << "M = Monster\n";
    std::cout << "E = Exit\n";
    std::cout << ". = Normal\n";
    std::cout << "? = Unknown\n";
    std::cout << "T = Trap\n";
    std::cout << "P = Poison (visited)\n";
    std::cout << "\n";

    // Status Information
    std::cout << "STATUS:\n";
    std::cout << "Health: " << player.getHealth() << "/100";
    if (player.isPoisoned()) {
        std::cout << " [POISONED]";
    }
    {
        std::string msg = player.consumeUiMessage();
        if (!msg.empty()) std::cout << msg;
    }
    std::cout << "\n";
    
    std::cout << "Moves Remaining: " << player.getMoves() << "/2\n";

    // Calculate distances
    int distToExit = player.getPosition().distanceTo(map.getExitPosition());
    int distToMonster = player.getPosition().distanceTo(monster.getPosition());
    
    std::cout << "Distance to Exit: " << distToExit << " steps\n";
    std::cout << "Monster Distance: " << distToMonster << " steps\n";
    std::cout << "\n";
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
