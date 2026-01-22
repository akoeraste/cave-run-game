#include "../headers/Map.h"
#include "../headers/Room.h"
#include "../headers/NormalRoom.h"
#include "../headers/PoisonRoom.h"
#include "../headers/TrapRoom.h"
#include "../headers/HealthRoom.h"
#include <random>
#include <ctime>
#include <iostream>

Map::Map(int rows, int cols)
    : rows(rows), cols(cols),
      exitPosition(cols - 1, rows - 1),
      rooms(rows, std::vector<Room*>(cols, nullptr)) {}

Map::~Map() {
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            delete rooms[y][x];
}

void Map::initialize() {
    // Random room generation with seed for reproducibility
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_int_distribution<int> healthPercentDist(1, 5);

    Position startPos(0, 0);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Position pos(x, y);
            // Start and exit must always be safe
            if ((pos.x == startPos.x && pos.y == startPos.y) ||
                (pos.x == exitPosition.x && pos.y == exitPosition.y)) {
                rooms[y][x] = new NormalRoom(pos);
            } else {
                // Random distribution: 20% poison, 20% trap, 10% health, 50% normal
                double roll = dist(rng);
                if (roll < 0.20) {
                    rooms[y][x] = new PoisonRoom(pos, 5);
                } else if (roll < 0.40) {
                    rooms[y][x] = new TrapRoom(pos, 20);
                } else if (roll < 0.50) {
                    // Random health percentage: 10%, 15%, 20%, 25%, or 30%
                    int healthPercentRoll = healthPercentDist(rng);
                    int healthPercentage = healthPercentRoll * 5 + 5;  // 10, 15, 20, 25, or 30
                    rooms[y][x] = new HealthRoom(pos, healthPercentage);
                } else {
                    rooms[y][x] = new NormalRoom(pos);
                }
            }
        }
    }
}

void Map::addRoom(Room* room) {
    Position pos = room->getPosition();
    if (isValidPosition(pos)) {
        delete rooms[pos.y][pos.x];  // Delete existing room if any
        rooms[pos.y][pos.x] = room;
    }
}

Room* Map::getRoom(const Position& pos) const {
    if (!isValidPosition(pos)) return nullptr;
    return rooms[pos.y][pos.x];
}

Position Map::getDimensions() const {
    return Position(cols, rows);
}

const Position& Map::getExitPosition() const {
    return exitPosition;
}

void Map::setExitPosition(const Position& pos) {
    exitPosition = pos;
}

bool Map::isValidPosition(const Position& pos) const {
    return pos.x >= 0 && pos.x < cols && pos.y >= 0 && pos.y < rows;
}

bool Map::isInside(const Position& pos) const {
    return isValidPosition(pos);
}

bool Map::isAdjacent(const Position& from, const Position& to) const {
    return from.isAdjacentTo(to);
}

void Map::display() const {
    for (int y = rows - 1; y >= 0; --y) {
        for (int x = 0; x < cols; ++x) {
            Position pos(x, y);
            Room* room = getRoom(pos);
            if (room) {
                std::cout << room->getSymbol() << ' ';
            } else {
                std::cout << "? ";
            }
        }
        std::cout << '\n';
    }
}

// Legacy methods for backward compatibility
const Position& Map::getStartPosition() const {
    static Position startPos(0, 0);
    return startPos;
}

int Map::getWidth() const { return cols; }
int Map::getHeight() const { return rows; }
