#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

class Button
{
    public:
        Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, const sf::Font& font, unsigned int charSize = 20);

        void processEvent(const sf::Event& event, const sf::Window& window);
        void draw(sf::RenderWindow& window) const;

        bool isClicked() const;

    private:
        sf::RectangleShape shape;
        sf::Text label;

        bool clicked = false;
};