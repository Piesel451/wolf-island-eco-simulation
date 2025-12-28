#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cstdint> 
#include <string>

class EditableLabel
{
	public:
		EditableLabel(const sf::Vector2f& position, const std::string& text, const sf::Font& font, unsigned int charSize = 20);

		void handleEvent(const sf::Event& event);

		bool hitTest(sf::Vector2f mousePos) const;

		void draw(sf::RenderWindow& window) const;

		void setString(const std::string& newText);
		std::string getString() const;

		void setFocused(bool f);
		bool isFocused();

		/*void updateDisplayString();*/

	private:
		sf::Text label;
		std::string editBuffer;
		bool focused = false;
};

