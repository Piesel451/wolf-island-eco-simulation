#include <iostream>
#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "Animal.h"
#include "Tile.h"

int main()
{
    srand(time(0));

    if (!Tile::loadTextures() || !Animal::loadTextures()) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstur!" << std::endl;
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode({ 1300, 800 }), "Wyspa wilków");

    int windowHeight = window.getSize().y;
    int windowWidth = window.getSize().x;

    int mapRows = 15;
    int mapCols = 15;
    float tileSize = std::min( //obliczanie rozmiaru kafelka bazuj¹c na rozmiarze mapy
        windowWidth / static_cast<float>(mapCols),
        windowHeight / static_cast<float>(mapRows)
    );
    float simulationSpeed = 0.1f;
    Simulation simulation(window, simulationSpeed, mapRows, mapCols, tileSize);
    simulation.run();
}