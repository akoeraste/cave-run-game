#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Player.h"
#include "Monster.h"

class Game {
private:
    Map map;
    Player player;
    Monster monster;
    bool gameOver;
    bool playerWon;

public:
    Game(int width, int height);

    void start();
    void run();

    void processPlayerTurn();
    void processMonsterTurn();
    void checkGameOver();

    const Position& getPlayerPosition() const;
    const Position& getMonsterPosition() const;
    const Position& getExitPosition() const;

    void render() const;
    void showGameOver() const;
};

#endif
