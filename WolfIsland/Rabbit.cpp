#include "Rabbit.h"
#include "Map.h"
#include "SimulationConfig.h"

Rabbit::Rabbit(sf::Vector2f position, Tile* currentTile, float startingEnergy, float maxEnergy, float energyLoss)
    : Animal(position, currentTile, true, startingEnergy, maxEnergy, energyLoss, 0.0f, AnimalType::Rabbit)
{
}

void Rabbit::move(Map& map) {
    Tile* targetTile = randomNearbyTile(map);

    if (targetTile == nullptr) {
        return;
    }
    else {
        auto [newRow, newCol] = targetTile->getRowCol();
        float tileSize = map.getTileSize();

        leaveCurrentTile();
        enterTile(targetTile);

        position = sf::Vector2f(newCol * tileSize, newRow * tileSize);
    }

}

std::unique_ptr<Animal> Rabbit::reproduce(Map& map, const SimulationConfig& config) {
    if (!currentTile) {
        return nullptr;
    }

    if (currentTile->occupantsCount() >= 3)
        return nullptr;

    float tileSize = map.getTileSize();
    auto [babyRow, babyCol] = currentTile->getRowCol();

    auto baby = std::make_unique<Rabbit>(
        sf::Vector2f(babyCol * tileSize, babyRow * tileSize),
        currentTile,
        config.rabbitStartingEnergy,
        config.rabbitMaxEnergy,
        config.rabbitEnergyLoss
    );

    baby->enterTile(currentTile);

    return baby;
}
