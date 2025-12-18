#pragma once
#include "Animal.h"

class Map;

class Rabbit : public Animal
{
	public:
        Rabbit(sf::Vector2f position, Tile* currentTile);
        void move(Map& map) override;
        std::unique_ptr<Animal> reproduce(Map& map) override;

	private:

};

