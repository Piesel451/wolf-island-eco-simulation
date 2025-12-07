#include "Map.h"
#include "Tile.h"
#include "Animal.h"

Map::Map(int rows, int cols, float tileSize) : rows(rows), cols(cols), tileSize(tileSize) {
	tiles.resize(rows);//create rows
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			tiles[i].emplace_back(j * tileSize, i * tileSize, tileSize, TileType::Grass);//TODO add hedge being spawned in lines across the map and water around the map
		}
	}
}

int Map::getRows() const {
	return rows;
}

int Map::getCols() const {
	return cols;
}

//returns a memory address of a specific tile
Tile* Map::getTile(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return nullptr;

	return &tiles[row][col];
}

std::pair<int, int> Map::getTileCoordsByOccupant(const Animal* a) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (tiles[i][j].getOccupant() == a) {
				return { i, j };
			}
		}
	}
	return { -1, -1 }; // nie znaleziono
}



//overloaded getTile(by occupant) to get animal that sits there

void Map::draw(sf::RenderWindow& window, std::vector<std::unique_ptr<Animal>>& animals) {
	for (auto& row : tiles) {
		for (auto& tile : row) { 
			tile.draw(window);
		}
	}
	for (const auto& a : animals) { //const, cannot modify the pointer itself
		a->draw(window, *this); //pass map so animal can use tileSize
	}
}

float Map::getTileSize() const{
	return tileSize;
}
