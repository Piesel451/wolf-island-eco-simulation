#pragma once
#include <SFML/Graphics.hpp>

class Animal;

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
		void clearOccupants();

		int occupantsCount() const;
		int getOccupantIndex(const Animal* a) const;

		sf::FloatRect getBounds() const;

		TileType getType() const;
		void setType(TileType newType);

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

