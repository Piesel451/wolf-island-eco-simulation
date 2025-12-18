#include "Wolf.h"
#include "Map.h"
#include <random>
#include "Rabbit.h"
#include <iostream>

Wolf::Wolf(sf::Vector2f position, Tile* currentTile, bool sex)
    : Animal(position, currentTile, sex, AnimalType::Wolf)
{
}

void Wolf::move(Map& map) {
    Tile* targetTile = chooseTargetTile(map);
    if (targetTile == nullptr) {//nie ma kogo goniæ

        targetTile = randomNearbyTile(map);
        if (targetTile == nullptr) {//nie ma gdzie sie ruszyc
            return;
        }
    }
    auto [newRow, newCol] = targetTile->getRowCol();
    float tileSize = map.getTileSize();

    leaveCurrentTile();
    enterTile(targetTile);


    position = sf::Vector2f(newCol * tileSize, newRow * tileSize);
}


Tile* Wolf::randomNearbyTile(Map& map) const {
    auto [row, col] = currentTile->getRowCol();

    std::pair<int, int> moves[8] = {
        {row - 1, col}, {row - 1, col + 1}, {row, col + 1}, {row + 1, col + 1},
        { row + 1, col }, {row + 1, col - 1}, {row, col - 1}, {row - 1, col - 1}
    };

    static std::mt19937 rng(std::random_device{}());
    std::shuffle(std::begin(moves), std::end(moves), rng); 

    for (auto [newRow, newCol] : moves) {
        Tile* newTile = map.getTile(newRow, newCol);

        if (!newTile || !newTile->isAccessible() || newTile->occupantsCount() >= 3) continue;

        if (newRow == row && newCol == col) {
            continue;
        }

        return newTile;
    }

    return nullptr;
}

Tile* Wolf::chooseTargetTile(Map& map) {
    auto [row, col] = currentTile->getRowCol();

    std::pair<int, int> moves[8] = {
    {row - 1, col}, {row - 1, col + 1}, {row, col + 1},
    {row + 1, col + 1}, {row + 1, col}, {row + 1, col - 1},
    {row, col - 1}, {row - 1, col - 1}
    };

    for (auto [newRow, newCol] : moves) {
        Tile* targetTile = map.getTile(newRow, newCol);

        if (!targetTile || !targetTile->isAccessible() || targetTile->occupantsCount() >= 3) continue;

        std::vector<Animal*> nearbyOccupants = targetTile->getOccupants();
        for (auto& nearbyOccupant : nearbyOccupants) {
            if (nearbyOccupant) {
                if (sex == true && nearbyOccupant->getSex() == false && nearbyOccupant->getType() == AnimalType::Wolf) {
                    return targetTile;//pogon za wilczyca
                }
                if (nearbyOccupant->getType() == AnimalType::Rabbit) {
                    return targetTile;//pogon za krolikiem
                }
            }
        }
    }
    return nullptr;
}

std::unique_ptr<Animal> Wolf::reproduce(Map& map) {
    bool sex = (rand() % 2 == 0); // 0 = samica, 1 = samiec

    if (!currentTile) {
        return nullptr;
    }

    if (currentTile->occupantsCount() >= 3)
        return nullptr;

    float tileSize = map.getTileSize();
    auto [babyRow, babyCol] = currentTile->getRowCol();

    auto baby = std::make_unique<Wolf>(
        sf::Vector2f(babyCol * tileSize, babyRow * tileSize),
        currentTile,
        sex
    );

    //od razu occupant na babyTile, ¿eby nie da³o siê tam wstawiæ kogoœ innego w tej samej fazie
    baby->enterTile(currentTile);

    return baby;
}

