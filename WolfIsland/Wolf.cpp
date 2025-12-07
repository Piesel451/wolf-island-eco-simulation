#include "Wolf.h"

Wolf::Wolf(sf::Vector2f position, float energy, bool alive, bool sex) 
	: Animal(position, energy, alive, sex, AnimalType::Wolf)
{

}
