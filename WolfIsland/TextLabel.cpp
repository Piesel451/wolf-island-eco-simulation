#include "TextLabel.h"

TextLabel::TextLabel(const sf::Vector2f& position, const std::string& text, const sf::Font& font, unsigned int charSize, sf::Color color) : label(font, text)
{
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(charSize);
    label.setFillColor(color);
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