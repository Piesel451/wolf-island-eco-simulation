#include "TextLabel.h"

TextLabel::TextLabel(const sf::Vector2f& position, const std::string& text, const sf::Font& font, unsigned int charSize) : label(font, text)
{
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(charSize);
    label.setFillColor(sf::Color::White);
    label.setPosition(position);
}

void TextLabel::draw(sf::RenderWindow& window) const{
    window.draw(label);
}

void TextLabel::setString(const std::string& newText) {
    label.setString(newText);
}

sf::Vector2f TextLabel::getPosition() const{
    return label.getPosition();
}