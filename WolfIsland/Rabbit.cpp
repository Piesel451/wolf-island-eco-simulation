#include "Rabbit.h"
#include "Map.h"
#include <random>

Rabbit::Rabbit(sf::Vector2f position, float energy, bool alive, bool sex)//TODO remove unnecesary fields from Rabbit constructor
    : Animal(position, energy, alive, sex, AnimalType::Rabbit)
{
}

//get tile by occupant(*this animal) it should return row and col of the tile then use tiles vactor from the map to determine which tile around animal is free to move then move
void Rabbit::move(Map& map) {
    auto [row, col] = map.getTileCoordsByOccupant(this);
    Tile* currTile = map.getTile(row, col);

    std::pair<int, int> moves[8] = {
        {row-1, col}, {row-1, col+1}, {row, col+1}, {row+1, col+1},
        {row+1, col}, {row+1, col-1}, {row, col-1}, {row-1, col-1}
     };

    static std::mt19937 rng(std::random_device{}()); //modern rand 
    std::shuffle(std::begin(moves), std::end(moves), rng); //shuffles elements of moves randomly based on rng()

    for (auto [newRow, newCol] : moves) {
        Tile* newTile = map.getTile(newRow, newCol);

        if (!newTile) continue;

        if (newTile->isAccessible() && newTile->getOccupant() == nullptr) {
            currTile->setOccupant(nullptr);
            newTile->setOccupant(this);
            setPosition(sf::Vector2f(newRow * map.getTileSize(), newCol * map.getTileSize()));
            return;
        }
    }
    
}

void Rabbit::eat(Map& map) {
}

void Rabbit::reproduce(Map& map) {
}
