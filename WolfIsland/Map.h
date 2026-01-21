#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Tile.h"

class Animal;

class Map
{
	public:
		Map(int rows, int cols, float tileSize);

		int getRows() const;
		int getCols() const;

		Tile* getTile(int row, int col);

		void draw(sf::RenderWindow& window, std::vector<std::unique_ptr<Animal>>& animals);
		void draw(sf::RenderWindow& window);

		void toggleTileType(sf::Vector2i mousePos);

		float getTileSize() const;
		float getWidth() const;

	private:
		int rows;
		int cols;
		float tileSize;
		std::vector<std::vector<Tile>> tiles;
		
};

