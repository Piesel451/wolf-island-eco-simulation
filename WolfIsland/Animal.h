#pragma once
#include <SFML/Graphics.hpp>

enum class AnimalType {
	Rabbit,
	Wolf,
};

class Map; //forward declaration
class Tile;

class Animal
{
	public:
		Animal(sf::Vector2f position, Tile* currentTile, bool sex, AnimalType type);


		virtual void move(Map& map) = 0;
		virtual std::unique_ptr<Animal> reproduce(Map& map) = 0;

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
		bool alive;
		bool sex; //true - samiec, false - samica
		sf::Sprite sprite;
		static sf::Texture rabbitTexture;
		static sf::Texture maleWolfTexture;
		static sf::Texture femaleWolfTexture;
		static const float maxEnergy;
		static const float energyCost;
		static const float energyGain;
};

