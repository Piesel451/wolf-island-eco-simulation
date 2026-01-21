#include "Animal.h"
#include "Map.h"
#include "ResourceLoader.h"
#include "resource.h"
#include <random>

//tekstury dla wszystkich instancji dziedzicz¹cyh po Animal
sf::Texture Animal::rabbitTexture;
sf::Texture Animal::maleWolfTexture;
sf::Texture Animal::femaleWolfTexture;

bool Animal::loadTextures() {
    bool rabbitLoaded = loadFromResource(rabbitTexture, IDR_RABBIT_PNG);
    bool maleWolfLoaded = loadFromResource(maleWolfTexture, IDR_WOLF_M_PNG);
    bool femaleWolfLoaded = loadFromResource(femaleWolfTexture, IDR_WOLF_F_PNG);

    return rabbitLoaded && maleWolfLoaded && femaleWolfLoaded;
}

Animal::Animal(sf::Vector2f position, Tile* currentTile, bool sex, float energy, float maxEnergy, float energyLoss, float energyGain, AnimalType type)
    : type(type),
    position(position),
    currentTile(currentTile),
    alive(true),
    sex(sex),
    energy(energy),
    maxEnergy(maxEnergy),
    energyLoss(energyLoss),
    energyGain(energyGain),
    sprite(
        (type == AnimalType::Rabbit) ? rabbitTexture :
        (type == AnimalType::Wolf && sex) ? maleWolfTexture :
        femaleWolfTexture
    )
{
    sprite.setPosition(position);
}

//to jest do refactoru nie potrzebny mi idx normalnie w zale¿noœci od tego ile zwiarzat na kafelku to tak skalujemy sa 3 opcje
void Animal::draw(sf::RenderWindow& window, const Map& map) {
    if (!currentTile) return;

    auto [factor, offset] = currentTile->getSlotTransform(this);

    sf::Vector2u texSize = sprite.getTexture().getSize();
    float tileSize = map.getTileSize();

    sprite.setScale(sf::Vector2f((tileSize / texSize.x) * factor, (tileSize / texSize.y) * factor));
    sprite.setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
    sprite.setPosition(currentTile->getBounds().position + offset);

    window.draw(sprite);
}

void Animal::consumeEnergy() {
    energy -= energyLoss;
    if (energy <= 0.0f) {
        energy = 0.0f;
        kill();
    }
}

void Animal::gainEnergy() {
    energy = std::min(energy + energyGain, maxEnergy);
}

bool Animal::isAlive() const {
    return alive;
}

void Animal::kill() {
    alive = false;
    if (currentTile) {
        currentTile->removeOccupant(this);
        currentTile = nullptr;
    }
}

bool Animal::getSex() const{
    return sex;
}

AnimalType Animal::getType() const {
    return type;
}

void Animal::setPosition(sf::Vector2f newPosition) {
    position = newPosition;
}

sf::Vector2f Animal::getPosition() const {
    return position;
}

Tile* Animal::getTile() const {
    return currentTile;
}

void Animal::setCurrTile(Tile* tile) {
    currentTile = tile;
}

void Animal::enterTile(Tile* tile) {
    if (!tile) {
        currentTile = nullptr;
        return;
    }
    tile->addOccupant(this);
    currentTile = tile;
}

void Animal::leaveCurrentTile() {
    if (!currentTile) return;
    currentTile->removeOccupant(this);
    currentTile = nullptr;
}

//u¿ywana do losowania pola królikom
Tile* Animal::randomNearbyTile(Map& map) const {
    auto [row, col] = currentTile->getRowCol();

    std::pair<int, int> moves[9] = {
        {row - 1, col}, {row - 1, col + 1}, {row, col + 1},
        {row + 1, col + 1},{row, col}, { row + 1, col },
        {row + 1, col - 1}, {row, col - 1}, {row - 1, col - 1}
    };

    static std::mt19937 rng(std::random_device{}());
    std::shuffle(std::begin(moves), std::end(moves), rng);

    for (auto [newRow, newCol] : moves) {
        Tile* newTile = map.getTile(newRow, newCol);
        if (!newTile || !newTile->isAccessible()) continue;

        if (newTile->occupantsCount() >= 3) continue;

        return newTile;
    }

    return nullptr;
}
