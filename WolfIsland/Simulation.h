#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Animal.h"
#include <vector>

class Simulation
{
	public:
		Simulation(sf::RenderWindow& window, float simulationSpeed, int mapRows, int mapCols, float tileSize);
		void run();
		void handleEvent();
		void initialize();
		void update();
		void spawnAnimals(int rabbitsNum, int maleWolfsNum, int femaleWolfsNum);

	private:
		sf::RenderWindow& window;
		Map map;
		std::vector<std::unique_ptr<Animal>> animals;
		//int mapWidth, mapHeight;
		//float tileSize;
		float simulationSpeed;
		template<typename T, typename ...Args> void spawnOneAnimal(Args && ...args);

};

