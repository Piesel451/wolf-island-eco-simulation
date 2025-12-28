#pragma once
#include <SFML/Graphics.hpp>
class Slider
{
    public:
        Slider() = default;
        Slider(sf::Vector2f position, float width);

        void handleEvent(const sf::Event& event, const sf::Window& window);
        void draw(sf::RenderWindow& window) const;

        float getValue() const; // 0.0 - 1.0
        void setValue(float simSpeed);

    private:
        sf::RectangleShape bar;
        sf::RectangleShape knob;

        bool dragging = false;
};

