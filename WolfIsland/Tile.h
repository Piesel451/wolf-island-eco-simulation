#pragma once
#include <SFML/Graphics.hpp>

class Animal; //forward declaration w celu unikniêcia pêtli(Animal.h wymagalo Map.h a ono wymaga³o Tile.h a Tile.h wymaga³o Animal.h i by³¹ pêtla

enum class TileType{
	Grass,
	Hedge
};

class Tile
{
	public:
		Tile(float x, float y, float size, TileType type);
		static bool loadTextures();
		void draw(sf::RenderWindow& window);
		bool isAccessible() const;
		Animal* getOccupant() const;
		void setOccupant(Animal* a);
		TileType getType() const;

	private:
		float x, y, size;
		TileType type;
		sf::Sprite sprite;
		Animal* occupant = nullptr; // nullptr = puste pole
		static sf::Texture hedgeTexture;
		static sf::Texture grassTexture;
		
};

