#include <iostream>
#include <SFML/Graphics.hpp>
#include "Simulation.h"
#include "ConfigMenu.h"

int main()
{
    std::srand(std::time(nullptr));

    if (!Tile::loadTextures() || !Animal::loadTextures() || !SideMenu::loadFont()) {
        std::cerr << "Nie uda³o siê za³adowaæ tekstur!" << std::endl;
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode({ 1300, 800 }), "Wyspa wilków");

    while (window.isOpen()) {
        window.setFramerateLimit(60);
        SimulationConfig config;
        ConfigMenu configMenu(window, config);
        bool isConfigured = configMenu.run();

        if (!isConfigured) {
            return 0;
        }

        Simulation simulation(window, config);
        simulation.start();
    }






    return 0;
}