#include "Slider.h"

Slider::Slider(sf::Vector2f position, float width) {
    bar.setSize(sf::Vector2f( width, 6.f ));
    bar.setPosition(position);
    bar.setFillColor(sf::Color::White);

    // Uchwyt
    knob.setSize(sf::Vector2f(10.f, 30.f));
    knob.setOrigin(knob.getSize() / 2.f);
    knob.setPosition(sf::Vector2f(position.x, position.y + bar.getSize().y / 2.f));
    knob.setFillColor(sf::Color::Red);
}

void Slider::handleEvent(const sf::Event& event, const sf::Window& window) {
    //dragging = false;

    if (event.is<sf::Event::MouseButtonPressed>() && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2f mousePos(sf::Mouse::getPosition(window));

        if (knob.getGlobalBounds().contains(mousePos))
            dragging = true;
    }

    if (event.is<sf::Event::MouseButtonReleased>() && event.getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left) {
        dragging = false;
    }

    if (event.is<sf::Event::MouseMoved>() && dragging) {
        sf::Vector2f mousePos(sf::Mouse::getPosition(window));

        float left = bar.getPosition().x;
        float right = left + bar.getSize().x;

        float x = mousePos.x;
        if (x < left) x = left;
        if (x > right) x = right;

        knob.setPosition(sf::Vector2f(x, knob.getPosition().y));
    }
}

void Slider::draw(sf::RenderWindow& window) const {
    window.draw(bar);
    window.draw(knob);
}

float Slider::getValue() const
{
    float left = bar.getPosition().x;
    float width = bar.getSize().x;

    return (knob.getPosition().x - left) / width;
}

void Slider::setValue(float value) {
    if (value < 0.f) value = 0.f;
    if (value > 1.f) value = 1.f;

    float left = bar.getPosition().x;
    float width = bar.getSize().x;

    float x = left + value * width;
    knob.setPosition(sf::Vector2f(x, knob.getPosition().y));
}
