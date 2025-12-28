#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

class TextLabel
{
	public:
		TextLabel(const sf::Vector2f& position, const std::string& text, const sf::Font& font, unsigned int charSize = 20);
		void draw(sf::RenderWindow& window) const;

		void setString(const std::string& newText);

		sf::Vector2f getPosition() const;
	private:
		sf::Text label;

};

