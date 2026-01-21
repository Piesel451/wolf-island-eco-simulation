#include "Simulation.h"
#include "Rabbit.h"
#include "Wolf.h"
#include "Tile.h"
#include "SideMenu.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <algorithm>


Simulation::Simulation(sf::RenderWindow& window, SimulationConfig& config)
	: window(window), config(config), simSpeed(config.simulationSpeed),
    map(//licznie tilesize rozbic do funkcji pozniej
        config.rows, config.cols, 
        std::min(
        window.getSize().y / static_cast<float>(config.rows),
        window.getSize().x / static_cast<float>(config.cols))
    ){}

//spawnuje jedno zwierze na kafelku spełniającym wymagania (helper function)
template<typename T, typename... Args> void Simulation::spawnOneAnimal(Args&&... args) { //Reference Collapsing l-value zmienia sie w zwykłą ref. , r-value pozostaje r-value reference (& zawsze dominuje)
    int randomRow = 0;
    int randomCol = 0;
    Tile* tile = nullptr;
    do {
        randomRow = rand() % map.getRows();
        randomCol = rand() % map.getCols();
        tile = map.getTile(randomRow, randomCol);
    } while (!tile->isAccessible() || tile->occupantsCount() >= 3);

    auto animal = std::make_unique<T>(
        sf::Vector2f(randomCol * map.getTileSize(), randomRow * map.getTileSize()), 
        tile,
        std::forward<Args>(args)...//perfect forwarding, zachowujemy kolejnosc argumentow
    );
    tile->addOccupant(animal.get());
    animals.emplace_back(std::move(animal));

}

void Simulation::spawnAnimals(int rabbitsNum, int maleWolfsNum, int femaleWolfsNum) {
    for (int i = 0; i < rabbitsNum; i++) {
        spawnOneAnimal<Rabbit>(
            config.rabbitStartingEnergy,
            config.rabbitMaxEnergy,
            config.rabbitEnergyLoss
        );
    }
    for (int i = 0; i < maleWolfsNum; i++) {
        spawnOneAnimal<Wolf>(
            true, 
            config.wolfStartingEnergy,
            config.wolfMaxEnergy,
            config.wolfEnergyLoss,
            config.wolfEnergyGain
        );
    }
    for (int i = 0; i < femaleWolfsNum; i++) {
        spawnOneAnimal<Wolf>(
            false,
            config.wolfStartingEnergy,
            config.wolfMaxEnergy,
            config.wolfEnergyLoss,
            config.wolfEnergyGain
        );
    }
}

void Simulation::resolveConflicts() {
    std::vector<std::unique_ptr<Animal>> newborns;

    for (int r = 0; r < map.getRows(); r++) {
        for (int c = 0; c < map.getCols(); c++) {
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
            //jesli wilków jest >2 to potomek nie może powstać (nie ma gdzie)
            if (rabbits.empty() && !maleWolves.empty() && !femaleWolves.empty()) {
                Wolf* male = static_cast<Wolf*>(maleWolves[0]);
                Wolf* female = static_cast<Wolf*>(femaleWolves[0]); //TODO moze nie trzeba static casta ? tylko normalnei Animal* ? (chyma mozna Animal* bo reproduce jest virtual)

                auto baby = female->reproduce(map, config);
                if (baby) {
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

        if (rand() % 100 < config.rabbitReproduceProb ) { //TODO dodać mozliwosc wpisywania np 15.7%, etc.. zamiast pełnych wartosci
            auto baby = animal->reproduce(map, config);

            if (baby) {
                newborns.push_back(std::move(baby));
            }
        }
    }

    for (auto& b : newborns)
        animals.push_back(std::move(b));
}

void Simulation::reset() {
    animals.clear();

    isRunning = false;
    isFirstLaunch = true;
    isRestart = false;

    for (int r = 0; r < map.getRows(); ++r) {
        for (int c = 0; c < map.getCols(); ++c) {
            Tile* tile = map.getTile(r, c);
            if (!tile) continue;

            tile->setType(TileType::Grass);
            tile->clearOccupants();
        }
    }

}

void Simulation::update() {
    // 1. WYKONYWANIE RUCHU (wszyscy wchodzą na Tile)
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

    // 5. USUWANIE MARTWYCH
    animals.erase(
        std::remove_if(animals.begin(), animals.end(),
            [](const auto& a) { return !a->isAlive(); }),
        animals.end()
    );
}

void Simulation::initialize() {
    window.clear();
    auto mapWidth = map.getWidth();
    auto windowWidth = window.getSize().x;
    auto windowHeight = window.getSize().y;

    sf::FloatRect sideMenuArea(
        sf::Vector2f(mapWidth, 0.f),
        sf::Vector2f(windowWidth - mapWidth, windowHeight)
    );

    sideMenu = std::make_unique<SideMenu>(sideMenuArea, config);

    map.draw(window);
    sideMenu->draw(window);
    window.display();
}

void Simulation::handleEvents() {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        sideMenu->processEvent(*event, window);

        if (sideMenu->isRunPressed()) {
            isRunning = true;
        }
        if (sideMenu->isStopPressed()) {
            isRunning = false;
        }
        if (sideMenu->isRestartPressed()) {
            isRestart = true;
        }
        if (sideMenu->isAddAnimalsPressed()) {
            auto animalsToSpawn = sideMenu->getAnimalsToSpawn();
            spawnAnimals(animalsToSpawn[0], animalsToSpawn[1], animalsToSpawn[2]);
        }

        //plot mozna dodawac i usuwac tylko przed wlaczeniem simki
        //TODO wywalic to do jakiejsc metody
        if (!isRunning && isFirstLaunch) {
            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                //kiedy kliknieto w kafelek trawa->zywoplot, zywoplot->trawa
                int rows = map.getRows();
                int cols = map.getCols();
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                Tile* tile;
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        tile = map.getTile(i, j);
                        if (tile) {
                            sf::FloatRect bounds = tile->getBounds();
                            if (bounds.contains(sf::Vector2f(mousePos.x, mousePos.y))) {
                                if (tile->getType() == TileType::Grass) {
                                    tile->setType(TileType::Hedge);
                                    tile->draw(window);
                                }
                                else {
                                    tile->setType(TileType::Grass);
                                    tile->draw(window);
                                }
                            }
                        }
                    }
                }
            }
        }    
    }
}

void Simulation::start() {
    window.setFramerateLimit(60);
    initialize();

    const float tickBase = 1.0f / 30.0f; //base sim tick 1/30
    float accumulator = 0.0f;

    sf::Clock clock;
    float frameTime = clock.restart().asSeconds();

    while (window.isOpen()){
        handleEvents();
        simSpeed = sideMenu->getSimSpeed();

        if (isRestart) {
            reset();
            return;
        }

        frameTime = clock.restart().asSeconds();
        if (frameTime > 0.25f) {// avoid spiral of death(if sim falls behind it needs even more time to catch up causing updates to pile up until it freezes
            frameTime = 0.25f; //if somehow sim missed more than 0,25 seconds lets assume that it only missed 0,25s and skip the rest
        }

        float tickLength = tickBase / simSpeed; //higher simulationSpeed => faster sim (shorter tick interval)

        if (isRunning) {
            // accumulator rośnie TYLKO gdy symulacja chodzi
            accumulator += frameTime;
            
            if (isFirstLaunch) {
                spawnAnimals(config.rabbitCount, config.maleWolvesCount, config.femaleWolvesCount);
                isFirstLaunch = false;
            }

            while (accumulator >= tickLength) { //update once, if frame takes too much time we subtract and update untill we catch up with sim
                update();
                accumulator -= tickLength;
            }
        }
        //RYSOWANIE MAPY I MENU (niezależnie od update żeby było płynnie a nie co tick)
        window.clear();
        map.draw(window, animals);
        sideMenu->update(animals);
        sideMenu->draw(window);
        window.display();
        
    }
}


//dodawanie w trakcie symulacji
