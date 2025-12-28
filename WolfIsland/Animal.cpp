#include "Animal.h"
#include "Map.h"
#include <random>

//tekstury dla wszystkich instancji dziedzicz¹cyh po Animal
sf::Texture Animal::rabbitTexture;
sf::Texture Animal::maleWolfTexture;
sf::Texture Animal::femaleWolfTexture;

bool Animal::loadTextures() {
    bool rabbitLoaded = rabbitTexture.loadFromFile("assets/rabbit.png");
    bool maleWolfLoaded = maleWolfTexture.loadFromFile("assets/maleWolf.png");
    bool femaleWolfLoaded = femaleWolfTexture.loadFromFile("assets/femaleWolf.png");
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
    float tileSize = map.getTileSize();
    auto texSize = sprite.getTexture().getSize();
    if (texSize.x == 0 || texSize.y == 0)
        return;

    // Bazowa pozycja = lewy górny róg kafelka (position to (col * tileSize, row * tileSize))
    sf::Vector2f tilePos = position;

    // Domyœlnie: pe³ny rozmiar kafelka
    float scaleX = tileSize / static_cast<float>(texSize.x);
    float scaleY = tileSize / static_cast<float>(texSize.y);
    sf::Vector2f finalPos = tilePos;

    int count = 1;
    int idx = 0;

    if (currentTile) {
        count = currentTile->occupantsCount();
        idx = currentTile->getOccupantIndex(this);
        if (idx < 0) idx = 0;
    }

    // Ustawienia zale¿ne od liczby occupantów
    switch (count) {
    case 1:
        // 1 zwierzak: pe³ny kafel
        // scaleX/Y ju¿ policzone, pozycja = lewy górny róg kafla
        finalPos = tilePos;
        break;

    case 2:
    {
        // 2 zwierzaki: dwa mniejsze obok siebie (50% wielkoœci)
        float factor = 0.5f;
        scaleX *= factor;
        scaleY *= factor;

        float spriteWidth = texSize.x * scaleX;
        float spriteHeight = texSize.y * scaleY;

        // centrowanie w pionie
        float y = tilePos.y + (tileSize - spriteHeight) * 0.5f;

        if (idx == 0) {
            // lewy
            float x = tilePos.x + (tileSize * 0.25f - spriteWidth * 0.5f);
            finalPos = { x, y };
        }
        else {
            // prawy
            float x = tilePos.x + (tileSize * 0.75f - spriteWidth * 0.5f);
            finalPos = { x, y };
        }
        break;
    }

    case 3:
    {
        // 3 zwierzaki: trzy jeszcze mniejsze (30%) w trójk¹cie
        float factor = 0.6f;
        scaleX *= factor;
        scaleY *= factor;

        float spriteWidth = texSize.x * scaleX;
        float spriteHeight = texSize.y * scaleY;

        // Góra-lewo, góra-prawo, dó³-œrodek
        if (idx == 0) {
            float x = tilePos.x + (tileSize * 0.25f - spriteWidth * 0.5f);
            float y = tilePos.y + (tileSize * 0.25f - spriteHeight * 0.5f);
            finalPos = { x, y };
        }
        else if (idx == 1) {
            float x = tilePos.x + (tileSize * 0.75f - spriteWidth * 0.5f);
            float y = tilePos.y + (tileSize * 0.25f - spriteHeight * 0.5f);
            finalPos = { x, y };
        }
        else { // idx == 2 lub wiêcej
            float x = tilePos.x + (tileSize * 0.5f - spriteWidth * 0.5f);
            float y = tilePos.y + (tileSize * 0.75f - spriteHeight * 0.5f);
            finalPos = { x, y };
        }
        break;
    }

    default:
        // >3 – awaryjnie traktujemy jak 3
    {
        float factor = 0.6f;
        scaleX *= factor;
        scaleY *= factor;

        float spriteWidth = texSize.x * scaleX;
        float spriteHeight = texSize.y * scaleY;

        float x = tilePos.x + (tileSize * 0.5f - spriteWidth * 0.5f);
        float y = tilePos.y + (tileSize * 0.5f - spriteHeight * 0.5f);
        finalPos = { x, y };
        break;
    }
    }

    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    sprite.setPosition(finalPos);
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
