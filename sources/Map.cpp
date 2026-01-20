#include "../headers/Map.h"
#include "../headers/Room.h"
#include "../headers/NormalRoom.h"
#include "../headers/PoisonRoom.h"
#include "../headers/TrapRoom.h"
#include <random>
#include <ctime>

Map::Map(int w, int h)
    : width(w), height(h),
      startPos(0, 0),
      exitPos(w - 1, h - 1),
      rooms(h, std::vector<Room*>(w, nullptr)) {}

Map::~Map() {
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            delete rooms[y][x];
}

void Map::initialize() {
    // Random room generation with seed for reproducibility
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            // Start and exit must always be safe
            if ((pos.x == startPos.x && pos.y == startPos.y) ||
                (pos.x == exitPos.x && pos.y == exitPos.y)) {
                rooms[y][x] = new NormalRoom(pos);
            } else {
                // Random distribution: 20% poison, 20% trap, 60% normal
                double roll = dist(rng);
                if (roll < 0.20) {
                    rooms[y][x] = new PoisonRoom(pos, 5);
                } else if (roll < 0.40) {
                    rooms[y][x] = new TrapRoom(pos, 20);
                } else {
                    rooms[y][x] = new NormalRoom(pos);
                }
            }
        }
    }
}

bool Map::isInside(const Position& pos) const {
    return pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height;
}

bool Map::isAdjacent(const Position& from, const Position& to) const {
    return from.isAdjacentTo(to);
}

Room* Map::getRoom(const Position& pos) const {
    if (!isInside(pos)) return nullptr;
    return rooms[pos.y][pos.x];
}

const Position& Map::getStartPosition() const {
    return startPos;
}

const Position& Map::getExitPosition() const {
    return exitPos;
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }
