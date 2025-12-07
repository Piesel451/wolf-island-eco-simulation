#include "Tile.h"
#include "Animal.h"

//textures for all Tile instances
sf::Texture Tile::grassTexture;
sf::Texture Tile::hedgeTexture;

bool Tile::loadTextures(){
	bool grassLoaded = grassTexture.loadFromFile("assets/grass.png");
	bool hedgeLoaded = hedgeTexture.loadFromFile("assets/hedge.png");
	return grassLoaded && hedgeLoaded;
}

Tile::Tile(float x, float y, float size, TileType type)
	: sprite(
		(type == TileType::Grass) ? grassTexture :
		hedgeTexture
	),type(type),x(x),y(y),size(size)
{
	sprite.setPosition(sf::Vector2f(x, y));

	auto texSize = sprite.getTexture().getSize();//pobieramy size oryginalnej tekstury
	sprite.setScale(sf::Vector2f(size / static_cast<float>(texSize.x), size / static_cast<float>(texSize.y)));//przeszkalowujemy teksture w zale¿noœci od jaki size obliczy³a Map
}

void Tile::draw(sf::RenderWindow& window){
	window.draw(sprite);
}

bool Tile::isAccessible() const { //TODO check for nullptr to prevent application crashes when *this is nullptr
	return (type == TileType::Grass);
}

Animal* Tile::getOccupant() const{
	return occupant;
}

void Tile::setOccupant(Animal* a){
	occupant = a;
}

TileType Tile::getType() const {
	return type;
}



