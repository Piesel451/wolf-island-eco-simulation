#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Simulation.h"
#include "Rabbit.h"
#include "Wolf.h"
#include "Tile.h"
#include <map>
#include <iostream>

Simulation::Simulation(sf::RenderWindow& window,float simulationSpeed, int mapRows, int mapCols, float tileSize) 
	: window(window), 
    simulationSpeed(simulationSpeed), 
    map(mapRows, mapCols, tileSize){}

//spawns one animal on unoccupied and accesible tiles randomly(helper function)
template<typename T, typename... Args> void Simulation::spawnOneAnimal(Args&&... args) {
    int randomRow = 0;
    int randomCol = 0;
    Tile* tile = nullptr;
    do {
        randomRow = rand() % map.getRows();
        randomCol = rand() % map.getCols();
        tile = map.getTile(randomRow, randomCol);
    } while (!tile->isAccessible() || tile->occupantsCount() >= 3);

    auto animal = std::make_unique<T>( //create animal object on the heap and returns it as unique_ptr
        sf::Vector2f(randomCol * map.getTileSize(), randomRow * map.getTileSize()), 
        tile,
        std::forward<Args>(args)...
    );
    tile->addOccupant(animal.get()); //get() returns the raw pointer without transferring ownership
    animals.emplace_back(std::move(animal));

}

void Simulation::spawnAnimals(int rabbitsNum, int maleWolfsNum, int femaleWolfsNum) {
    for (int i = 0; i < rabbitsNum; i++) {
        spawnOneAnimal<Rabbit>();
    }
    for (int i = 0; i < maleWolfsNum; i++) {
        spawnOneAnimal<Wolf>(true);
    }
    for (int i = 0; i < femaleWolfsNum; i++) {
        spawnOneAnimal<Wolf>(false);
    }
}

void Simulation::resolveConflicts() {
    std::vector<std::unique_ptr<Animal>> newborns;

    for (int r = 0; r < map.getRows(); ++r) {
        for (int c = 0; c < map.getCols(); ++c) {
            Tile* tile = map.getTile(r, c);
            if (!tile || !tile->isAccessible()) continue;

            auto& occ = tile->getOccupants();
            if (occ.empty()) continue;

            //dzielimy na typy
            std::vector<Animal*> rabbits;
            std::vector<Animal*> maleWolves;
            std::vector<Animal*> femaleWolves;

            for (Animal* a : occ) {
                if (!a->isAlive()) continue;
                if (a->getType() == AnimalType::Rabbit) {
                    rabbits.push_back(a);
                }
                else {
                    if (a->getSex())
                        maleWolves.push_back(a);
                    else
                        femaleWolves.push_back(a);
                }
            }

            // --- PRZYPADEK: WILK(i) + KRÓLIK(i) NA TYM SAMYM TILE ---
            if (!rabbits.empty() && (!maleWolves.empty() || !femaleWolves.empty())) {
                std::cout << "kaput krolik.\n";

                std::vector<Animal*> allWolves;
                allWolves.reserve(maleWolves.size() + femaleWolves.size());
                allWolves.insert(allWolves.end(), maleWolves.begin(), maleWolves.end());
                allWolves.insert(allWolves.end(), femaleWolves.begin(), femaleWolves.end());

                // losowy wilk zabija losowego królika
                Animal* killer = allWolves[rand() % allWolves.size()];
                Animal* victim = rabbits[rand() % rabbits.size()];

                killer->gainEnergy();
                victim->kill();
            }

            // --- WILKI OBU PŁCI (BEZ KRÓLIKÓW) ---
            //jesli wilków będzie >2 to potomek nie może powstać(nie ma gdzie)
            if (rabbits.empty() && !maleWolves.empty() && !femaleWolves.empty()) {
                //wybieramy 1 pare
                Wolf* male = static_cast<Wolf*>(maleWolves[0]);
                Wolf* female = static_cast<Wolf*>(femaleWolves[0]);

                auto baby = female->reproduce(map);
                if (baby) {
                    std::cout << "potomek wilkow.\n";
                    newborns.push_back(std::move(baby));
                }
            }
        }
    }

    for (auto& b : newborns) {
        animals.push_back(std::move(b));
    }
}

void Simulation::rabbitReproduction(Map& map) {
    std::vector<std::unique_ptr<Animal>> newborns;

    for (auto& animal : animals) {
        if (!animal->isAlive()) continue;
        if (animal->getType() != AnimalType::Rabbit) continue;

        if (rand() % 100 < 15 ) {
            auto baby = animal->reproduce(map);

            if (baby) {
                std::cout << "potomek krolikow.\n";
                newborns.push_back(std::move(baby));
            }
        }
    }

    for (auto& b : newborns)
        animals.push_back(std::move(b));
}

void Simulation::update() {
    std::cout << "Update\n";
    // 1. WYKONYWANIE RUCHU (wszyscy wchodzą na nextTile)
    for (auto& animal : animals) {
        if (animal->isAlive()) {
            animal->move(map);
        }
        animal->consumeEnergy();
    }

    // 2. ROZSTRZYGANIE KONFLIKTÓW
    resolveConflicts();

    // 4. ROZMNAZANIE KROLIKOW I WILKOW(WRZUCIC DO FUNKCJI)
    rabbitReproduction(map);

    // 5. USUWANIE MARTWYCH moze dac przed wykonywaniem ruchu ? XD
    animals.erase(
        std::remove_if(animals.begin(), animals.end(),
            [](const auto& a) { return !a->isAlive(); }),
        animals.end()
    );

    // 6. RYSOWANIE
    window.clear();
    map.draw(window, animals);
    window.display();
}

void Simulation::initialize() {
    window.clear();
    spawnAnimals(8,3,3); //TODO add selecting number of animals and energy consumption before running sim(in menu) 
    map.draw(window);
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
            update();
            accumulator -= tickLength;
        }
        
    }
}

//dodawanie w trakcie symulacji
