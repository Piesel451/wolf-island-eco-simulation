#pragma once
#include "Animal.h"

class Map;
class Rabbit;

class Wolf : public Animal
{
    public:
        Wolf(sf::Vector2f position, Tile* currentTile, bool sex);
        void move(Map& map) override;
        std::unique_ptr<Animal> reproduce(Map& map) override;
        Tile* chooseTargetTile(Map& map);
        Tile* randomNearbyTile(Map& map) const override;

    private:

};

