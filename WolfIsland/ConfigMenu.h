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
#include <memory>
#include <vector>

class ConfigMenu
{
public:
    ConfigMenu(sf::RenderWindow& window, SimulationConfig& config);

    // zwraca true jeœli u¿ytkownik wcisn¹³ Start, false jeœli zamkn¹³.
    bool run();

private:
    sf::RenderWindow& window;
    SimulationConfig& config;

    std::vector<TextLabel> headers;
    std::vector<TextLabel> labels;
    std::vector<EditableLabel> values;
    std::vector<Button> buttons;

    EditableLabel* focusedLabel;
    std::unique_ptr<TextLabel> errorLabel;

    void initLayout();
    void initValues();
    void handleEvents(bool& done);
};

