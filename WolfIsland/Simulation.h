#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Map.h"
#include "Animal.h"
#include "SimulationConfig.h"
#include "SideMenu.h"

class Simulation
{
	public:
		Simulation(sf::RenderWindow& window, SimulationConfig& config);

		void start();

	private:
		void handleEvents();
		void initialize();
		void update();

		void spawnAnimals(int rabbitsNum, int maleWolfsNum, int femaleWolfsNum);
		template<typename T, typename ...Args> void spawnOneAnimal(Args && ...args);

		void resolveConflicts();
		void rabbitReproduction(Map& map);

		void reset();

		sf::RenderWindow& window;
		Map map;
		std::vector<std::unique_ptr<Animal>> animals;
		SimulationConfig& config;
		std::unique_ptr<SideMenu> sideMenu;

		bool isRunning = false;
		bool isRestart = false;
		bool isFirstLaunch = true;

		float simSpeed;
};

