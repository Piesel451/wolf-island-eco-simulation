#include "Tile.h"
#include "Animal.h"
#include "ResourceLoader.h"
#include "resource.h"
#include <algorithm>

//tekstury dla wszystkich instancji Tile
sf::Texture Tile::grassTexture;
sf::Texture Tile::hedgeTexture;

bool Tile::loadTextures(){
	bool grassLoaded = loadFromResource(grassTexture, IDR_GRASS_B_PNG);
	bool hedgeLoaded = loadFromResource(hedgeTexture, IDR_HEDGE_PNG);
	return grassLoaded && hedgeLoaded;
}

Tile::Tile(int row, int col, sf::Vector2f position, float size, TileType type)
	: sprite(
		(type == TileType::Grass) ? grassTexture :
		hedgeTexture
	),type(type), row(row), col(col), position(position), size(size)
{
	sprite.setPosition(position);

	auto texSize = sprite.getTexture().getSize();
	sprite.setScale(sf::Vector2f(size / static_cast<float>(texSize.x), size / static_cast<float>(texSize.y)));
}

const std::vector<Animal*>& Tile::getOccupants() const {
	return occupants;
}

void Tile::addOccupant(Animal* a) {
	if (!a) return;
	occupants.push_back(a);
}

void Tile::removeOccupant(Animal* a) {
	if (!a) return;
	auto occupant = std::remove(occupants.begin(), occupants.end(), a);
	occupants.erase(occupant, occupants.end());
}

void Tile::clearOccupants() {
	occupants.clear();
}

int Tile::occupantsCount() const {
	return static_cast<int>(occupants.size());
}

int Tile::getOccupantIndex(const Animal* a) const{
	for (int i = 0; i < static_cast<int>(occupants.size()); i++) {
		if (occupants[i] == a)
			return i;
	}
	return -1;
}

sf::FloatRect Tile::getBounds() const {
	return sprite.getGlobalBounds();
}

void Tile::draw(sf::RenderWindow& window) {
	if (type == TileType::Grass) {
		sprite.setTexture(grassTexture);
	}
	else {
		sprite.setTexture(hedgeTexture);
	}

	window.draw(sprite);
}

bool Tile::isAccessible() const {
	return (type == TileType::Grass);
}

TileType Tile::getType() const {
	return type;
}

void Tile::setType(TileType newType) {
	type = newType;
}

std::pair<int, int> Tile::getRowCol() {
	return { row, col };
}

// zwraca wspó³czynnik skali i przesuniêcie
std::pair<float, sf::Vector2f> Tile::getSlotTransform(const Animal* a) const {
	int occ = occupants.size();
	int myIdx = getOccupantIndex(a);
	float factor = 1.0f;
	sf::Vector2f offset(size * 0.5f, size * 0.5f);

	if (occ == 2) {
		factor = 0.5f;
		offset.x = (myIdx == 0) ? size * 0.25f : size * 0.75f;
	}
	else if (occ >= 3) {
		factor = 0.5f;
		if (myIdx == 0)      
			offset = sf::Vector2f( size * 0.25f, size * 0.25f );
		else if (myIdx == 1) 
			offset = sf::Vector2f( size * 0.75f, size * 0.25f );
		else                 
			offset = sf::Vector2f( size * 0.50f, size * 0.75f );
	}
	return { factor, offset };
}



