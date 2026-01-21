#include "Map.h"
#include "Animal.h"

Map::Map(int rows, int cols, float tileSize) : rows(rows), cols(cols), tileSize(tileSize) {
	tiles.resize(rows);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			tiles[i].emplace_back(i, j, sf::Vector2f(j * tileSize, i * tileSize), tileSize, TileType::Grass);
		}
	}
}

int Map::getRows() const {
	return rows;
}

int Map::getCols() const {
	return cols;
}

Tile* Map::getTile(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return nullptr;

	return &tiles[row][col];
}

void Map::draw(sf::RenderWindow& window, std::vector<std::unique_ptr<Animal>>& animals) {
	for (auto& row : tiles) {
		for (auto& tile : row) { 
			tile.draw(window);
		}
	}
	for (const auto& a : animals) {
		a->draw(window, *this);
	}
}

void Map::draw(sf::RenderWindow& window) {
	for (auto& row : tiles) {
		for (auto& tile : row) {
			tile.draw(window);
		}
	}
}

float Map::getTileSize() const{
	return tileSize;
}

float Map::getWidth() const {
	return cols * tileSize;
}
