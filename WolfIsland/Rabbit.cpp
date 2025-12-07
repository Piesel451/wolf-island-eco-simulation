#include "Rabbit.h"
#include "Map.h"

Rabbit::Rabbit(sf::Vector2f position, float energy, bool alive, bool sex)//TODO remove unnecesary fields from Rabbit constructor
    : Animal(position, energy, alive, sex, AnimalType::Rabbit)
{

}