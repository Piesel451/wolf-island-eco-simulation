#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include "SimulationConfig.h"
#include "Button.h"
#include "TextLabel.h"
#include "EditableLabel.h"
#include "SideMenu.h"
#include <string>
#include <sstream>

class ConfigMenu
{
public:
    ConfigMenu(sf::RenderWindow& window, SimulationConfig& config);

    // Zwraca true jeœli u¿ytkownik wcisn¹³ Start, false jeœli ESC / zamkn¹³.
    bool run();

private:
    sf::RenderWindow& window;
    SimulationConfig& config;

    std::vector<TextLabel> headers;
    std::vector<TextLabel> labels;
    std::vector<EditableLabel> values;
    std::vector<Button> buttons;

    EditableLabel* focusedLabel;

    void initLayout();
    void initValues();
    void changeValue(int delta);
    void handleEvents(bool& done);
};

