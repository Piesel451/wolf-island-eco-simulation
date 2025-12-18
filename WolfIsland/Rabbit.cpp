#include "Rabbit.h"
#include "Map.h"
#include <random>
#include <iostream>

Rabbit::Rabbit(sf::Vector2f position, Tile* currentTile)
    : Animal(position, currentTile, true, AnimalType::Rabbit)
{
}

//w pewnym momencie przestaja chciec sie ruszac dziwne
//przypatrzeæ siê moze po prostu jest bardzo ma³a szansa na ruch przy zape³nionej mapie
void Rabbit::move(Map& map) {
    Tile* targetTile = randomNearbyTile(map);

    if (targetTile == nullptr) {
        return;
    }
    else {
        auto [newRow, newCol] = targetTile->getRowCol();
        float tileSize = map.getTileSize();

        leaveCurrentTile();
        enterTile(targetTile);

        position = sf::Vector2f(newCol * tileSize, newRow * tileSize);
    }

}

std::unique_ptr<Animal> Rabbit::reproduce(Map& map) {
    if (!currentTile) {
        return nullptr;
    }

    if (currentTile->occupantsCount() >= 3)
        return nullptr;

    float tileSize = map.getTileSize();
    auto [babyRow, babyCol] = currentTile->getRowCol();

    auto baby = std::make_unique<Rabbit>(
        sf::Vector2f(babyCol * tileSize, babyRow * tileSize),
        currentTile
    );

    //od razu occupant na babyTile, ¿eby nie da³o siê tam wstawiæ kogoœ innego w tej samej fazie
    baby->enterTile(currentTile);

    return baby;
}
