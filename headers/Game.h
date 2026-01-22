#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Player.h"
#include "Monster.h"

class Character;

class Game {
private:
    Map map;
    Player player;
    Monster monster;
    Character* activeCharacter;
    bool gameOver;
    bool playerWon;
    int turnCount;

public:
    Game(int rows, int cols);

    void start();
    void run();

    Position getActivePosition() const;
    bool isGameOver() const;
    bool playerWins() const;
    
    void playerTurn();
    void monsterTurn();
    void switchTurn();
    void checkGameOver();
    
    // Legacy method names for backward compatibility
    void processPlayerTurn();
    void processMonsterTurn();

    const Position& getPlayerPosition() const;
    const Position& getMonsterPosition() const;
    const Position& getExitPosition() const;

    void displayMap() const;
    void displayGameOver() const;
    
    // Legacy method names for backward compatibility
    void render() const;
    void showGameOver() const;
};

#endif
