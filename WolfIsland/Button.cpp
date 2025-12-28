#include "Button.h"

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& text, const sf::Font& font, unsigned int charSize) : label(font, text)
{
    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(sf::Color::White);

    label.setCharacterSize(charSize);
    label.setFillColor(sf::Color::Black);

    // Wyœrodkowanie tekstu
    sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin(bounds.size / 2.f);
    label.setPosition(sf::Vector2f(position.x + size.x / 2.f, position.y + size.y / 2.f - 5.f));
}

void Button::processEvent(const sf::Event& event, const sf::Window& window) {
    clicked = false;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect bounds = shape.getGlobalBounds();

    if (bounds.contains(sf::Vector2f(mousePos.x, mousePos.y)))
    {
        shape.setFillColor(sf::Color(200, 200, 255));

        if (event.is<sf::Event::MouseButtonPressed>() && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
            clicked = true;
        }
    }
    else
    {
        shape.setFillColor(sf::Color::White);
    }
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(label);
}

bool Button::isClicked() const {
    return clicked;
}
