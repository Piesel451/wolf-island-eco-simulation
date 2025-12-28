#include "EditableLabel.h"
#include <iostream>

EditableLabel::EditableLabel(const sf::Vector2f& position, const std::string& text, const sf::Font& font, unsigned int charSize) : label(font, text)
{
    label.setFont(font);
    label.setString(text);
    label.setCharacterSize(charSize);
    label.setFillColor(sf::Color::White);
    label.setPosition(position);
}

//obs³uguje input z klawiatury tylko wtedy jeœli label ma focus
void EditableLabel::handleEvent(const sf::Event& event) {
    if (!focused)
        return;

    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
        uint32_t ch = textEntered->unicode;;

        if (ch >= 32 && ch < 127) {
            editBuffer += static_cast<char>(ch);
            label.setString(editBuffer);
        }
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Backspace && !editBuffer.empty()) {
            editBuffer.pop_back();
            label.setString(editBuffer);
        }
    }
}

bool EditableLabel::hitTest(sf::Vector2f mousePos) const {
    return label.getGlobalBounds().contains(mousePos);
}


void EditableLabel::setFocused(bool f) {
    focused = f;
    label.setFillColor(f ? sf::Color(100, 200, 255) : sf::Color::White);

    if (focused) {
        // start edycji
        editBuffer = label.getString();
    }
    else {
        // KONIEC edycji
        if (editBuffer.empty()) {
            editBuffer = "----";
        }
        label.setString(editBuffer);
    }
}

bool EditableLabel::isFocused(){
    return focused;
}

void EditableLabel::draw(sf::RenderWindow& window) const {
    window.draw(label);
}

void EditableLabel::setString(const std::string& newText) {
    label.setString(newText);
}

std::string EditableLabel::getString() const {
    return label.getString();
}
