#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "Simulation.h"
#include "Rabbit.h"
#include "Wolf.h"
#include "Tile.h"

Simulation::Simulation(sf::RenderWindow& window,float simulationSpeed, int mapRows, int mapCols, float tileSize) 
	: window(window), simulationSpeed(simulationSpeed), map(mapRows, mapCols, tileSize) {

}

//spawns one animal on unoccupied and accesible tiles randomly(helper function)
template<typename T, typename... Args> void Simulation::spawnOneAnimal(Args&&... args) {
    int randomRow = 0;
    int randomCol = 0;
    Tile* tile = nullptr;
    do {
        randomRow = rand() % map.getRows();
        randomCol = rand() % map.getCols();
        tile = map.getTile(randomRow, randomCol);
    } while (!tile->isAccessible() || tile->getOccupant() != nullptr); //randomise until found accesible tile and tile is not occupied

    auto animal = std::make_unique<T>( //create animal object on the heap and returns it as unique_ptr
        sf::Vector2f(randomCol * map.getTileSize(), randomRow * map.getTileSize()), 
        std::forward<Args>(args)...
    );
    tile->setOccupant(animal.get()); //get() returns the raw pointer without transferring ownership
    animals.emplace_back(std::move(animal));

}

void Simulation::spawnAnimals(int rabbitsNum, int maleWolfsNum, int femaleWolfsNum) {
    for (int i = 0; i < rabbitsNum; i++) {
        spawnOneAnimal<Rabbit>(100.0f, true, true);
    }
    for (int i = 0; i < maleWolfsNum; i++) {
        spawnOneAnimal<Wolf>(100.0f, true, true);
    }
    for (int i = 0; i < femaleWolfsNum; i++) {
        spawnOneAnimal<Wolf>(100.0f, true, false);
    }
}

void Simulation::update() {
    window.clear();
    map.draw(window, animals);
    window.display();
}

void Simulation::initialize() {
    window.clear();
    spawnAnimals(12,4,4); //todo add selecting number of animals before running sim
    map.draw(window, animals);
    window.display();
}

void Simulation::handleEvent() {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Simulation::run() {
    window.setFramerateLimit(60);
    initialize();

    const float tickBase = 1.0f / 30.0f; //base sim tick 1/30
    float accumulator = 0.0f;

    sf::Clock clock;
    float frameTime = clock.restart().asSeconds();

    while (window.isOpen()){
        handleEvent();
        frameTime = clock.restart().asSeconds();
        if (frameTime > 0.25f) {// avoid spiral of death(if sim falls behind it needs even more time to catch up causing updates to pile up until it freezes
            frameTime = 0.25f; //if somehow sim missed more than 0,25 seconds lets assume that it only missed 0,25s and skip the rest
        }

        accumulator += frameTime;

        float tickLength = tickBase / simulationSpeed; //higher simulationSpeed => faster sim (shorter tick interval)

        while (accumulator >= tickLength) { //update once, if frame takes too much time we subtract and update untill we catch up with sim
            std::cout << "One updae" << std::endl;
            update();
            accumulator -= tickLength;
        }
        
    }
}

