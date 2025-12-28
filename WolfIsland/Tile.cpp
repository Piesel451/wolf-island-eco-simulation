#include "Tile.h"
#include "Animal.h"
#include <algorithm>

//tekstury dla wszystkich instancji Tile
sf::Texture Tile::grassTexture;
sf::Texture Tile::hedgeTexture;

bool Tile::loadTextures(){
	bool grassLoaded = grassTexture.loadFromFile("assets/grass(borders).png");
	bool hedgeLoaded = hedgeTexture.loadFromFile("assets/hedge.png");
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



