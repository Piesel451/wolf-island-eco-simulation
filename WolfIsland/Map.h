#pragma once
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Animal;

class Map
{
	public:
		Map(int rows, int cols, float tileSize);
		int getRows() const;
		int getCols() const;
		Tile* getTile(int row, int col);
		/*std::pair<int, int> getTileCoordsByOccupant(const Animal* a);*/
		void draw(sf::RenderWindow& window, std::vector<std::unique_ptr<Animal>>& animals);
		void draw(sf::RenderWindow& window);
		float getTileSize() const;
	private:
		int rows, cols;
		float tileSize;
		std::vector<std::vector<Tile>> tiles;
		std::vector<std::unique_ptr<Animal>>animals;
		
};

