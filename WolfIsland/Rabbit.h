#pragma once
#include "Animal.h"

struct SimulationConfig;

class Rabbit : public Animal
{
	public:
        Rabbit(sf::Vector2f position, Tile* currentTile, float startingEnergy, float maxEnergy, float energyLoss);
        void move(Map& map) override;
        std::unique_ptr<Animal> reproduce(Map& map, const SimulationConfig& config) override;
};

