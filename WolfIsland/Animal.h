#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Map;
class Tile;
struct SimulationConfig;


enum class AnimalType {
	Rabbit,
	Wolf,
};

class Animal
{
	public:
		Animal(sf::Vector2f position, Tile* currentTile, bool sex, float startingEnergy, float maxEnergy, float energyLoss, float energyGain, AnimalType type);
		virtual ~Animal() = default; //potrzebny defaultowy destruktor wirtualny, inaczej jesli kompilator u¿yje zwyk³êgo domyœlnego
		//destruktora to wygeneruje ~Animal(). niszczenie przez Animal* nie wywo³a pe³nego destruktora obiektu pochodnego, 
		//jeœli w Animal nie ma virtual ~Animal()

		virtual void move(Map& map) = 0;
		virtual std::unique_ptr<Animal> reproduce(Map& map, const SimulationConfig& config) = 0;

		//wspólna domyœlna implementacja nadpisywana przez klasê Wolf
		virtual Tile* randomNearbyTile(Map& map) const;

		//Wspólne metody
		void draw(sf::RenderWindow& window, const Map& map);
		void consumeEnergy();
		void gainEnergy();

		bool isAlive() const;
		void kill();

		bool getSex() const;
		AnimalType getType() const;

		void setPosition(sf::Vector2f newPosition);
		sf::Vector2f getPosition() const;

		Tile* getTile() const;
		void setCurrTile(Tile* tile);
			
		void enterTile(Tile* tile);
		void leaveCurrentTile(); ;
		
		static bool loadTextures();

	protected:
		AnimalType type;
		sf::Vector2f position;
		Tile* currentTile;
		float energy;
		float maxEnergy;
		float energyGain;
		float energyLoss;
		bool alive;
		bool sex; //true - samiec, false - samica
		sf::Sprite sprite;

		static sf::Texture rabbitTexture;
		static sf::Texture maleWolfTexture;
		static sf::Texture femaleWolfTexture;
};

