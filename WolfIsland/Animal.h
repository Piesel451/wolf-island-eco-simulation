#pragma once
#include <SFML/Graphics.hpp>

enum class AnimalType {
	Rabbit,
	Wolf,
};

class Map; //forward declaration

class Animal
{
	public:
		Animal(sf::Vector2f position, float energy, bool alive, bool sex, AnimalType type);


		virtual void move(Map& map) = 0;
		virtual void eat(Map& map) = 0;
		virtual void reproduce(Map& map) = 0;

		//Wspólne metody
		void draw(sf::RenderWindow& window, const Map& map);
		void update(Map& map); //idk if its useful
		bool isAlive() const;
		bool getSex() const;
		AnimalType getType() const;
		sf::Vector2f getPosition() const;

		static bool loadTextures();

	protected:
		AnimalType type;
		sf::Vector2f position;
		float energy;
		bool alive;
		bool sex; //true - samiec, false - samica
		sf::Sprite sprite;
		static sf::Texture rabbitTexture;
		static sf::Texture maleWolfTexture;
		static sf::Texture femaleWolfTexture;

};

