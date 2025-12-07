#pragma once
#include "Animal.h"

class Map;

class Wolf : public Animal
{
    public:
        Wolf(sf::Vector2f position, float energy, bool alive, bool sex);
        void move(Map& map) override {}
        void eat(Map& map) override {}
        void reproduce(Map& map) override {}

    private:
};

