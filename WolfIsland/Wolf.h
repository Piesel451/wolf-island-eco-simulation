#pragma once
#include "Animal.h"
#include <array>
#include <utility>

struct SimulationConfig;

class Wolf : public Animal
{
    public:
        Wolf(sf::Vector2f position, Tile* currentTile, bool sex, float startingEnergy, float maxEnergy, float energyLoss, float energyGain);
        void move(Map& map) override;
        std::unique_ptr<Animal> reproduce(Map& map, const SimulationConfig& config) override;

    private:
        Tile* chooseTargetTile(Map& map);
        Tile* randomNearbyTile(Map& map) const override;
        std::array<std::pair<int, int>, 8> getNeighbourCoords() const;

};

