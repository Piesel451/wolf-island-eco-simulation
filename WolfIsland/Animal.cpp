#include "Animal.h"
#include "Map.h"

//textures for all instances that inherit from Animal
sf::Texture Animal::rabbitTexture;
sf::Texture Animal::maleWolfTexture;
sf::Texture Animal::femaleWolfTexture;

bool Animal::loadTextures() {
    bool rabbitLoaded = rabbitTexture.loadFromFile("assets/rabbit.png");
    bool maleWolfLoaded = maleWolfTexture.loadFromFile("assets/maleWolf.png");
    bool femaleWolfLoaded = femaleWolfTexture.loadFromFile("assets/femaleWolf.png");
    return rabbitLoaded && maleWolfLoaded && femaleWolfLoaded;
}

Animal::Animal(sf::Vector2f position, float energy, bool alive, bool sex, AnimalType type) 
    : position(position), energy(energy), alive(alive), sex(sex), type(type), 
    sprite(
        (type == AnimalType::Rabbit) ? rabbitTexture :
        (type == AnimalType::Wolf && sex == true) ? maleWolfTexture :
        femaleWolfTexture
    ) 
{
};

void Animal::draw(sf::RenderWindow& window, const Map& map) {
    //later can be moved into constructor but reference to map have to be passed
    float size = map.getTileSize();
    auto texSize = sprite.getTexture().getSize();

    sprite.setScale(sf::Vector2f((size / static_cast<float>(texSize.x)), (size / static_cast<float>(texSize.y))));
    sprite.setPosition(position);

    window.draw(sprite);
}

void Animal::update(Map& map) {
    // przyk³adowa implementacja, np. zmniejsz energiê
}

bool Animal::isAlive() const {
    return alive;
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
