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
		Tile(int row, int col, sf::Vector2f position, float size, TileType type);
		static bool loadTextures();
		void draw(sf::RenderWindow& window);
		bool isAccessible() const;

		const std::vector<Animal*>& getOccupants() const;
		void addOccupant(Animal* a);
		void removeOccupant(Animal* a);
		int  occupantsCount() const;
		int getOccupantIndex(const Animal* a) const;

		TileType getType() const;
		std::pair<int, int> getRowCol();

	private:
		int row, col;
		float size;
		sf::Vector2f position;
		TileType type;
		sf::Sprite sprite;

		std::vector<Animal*> occupants;

		static sf::Texture hedgeTexture;
		static sf::Texture grassTexture;
		
};

