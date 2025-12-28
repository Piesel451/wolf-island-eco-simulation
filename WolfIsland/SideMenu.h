#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>
#include <vector> 
#include <array>
#include <sstream>
#include "Button.h"
#include "TextLabel.h"
#include "EditableLabel.h"
#include "Slider.h"
#include "SimulationConfig.h"

class Animal;

class SideMenu
{
    public:
        SideMenu(sf::FloatRect area, SimulationConfig& config);

        void processEvent(const sf::Event& event, sf::Window& window);
        void update(std::vector<std::unique_ptr<Animal>>& animals);
        void draw(sf::RenderWindow& window) const;

        static bool loadFont();
        static sf::Font& getFont();

        bool isRunPressed() const;
        bool isStopPressed() const;
        bool isAddAnimalsPressed() const;
        bool isRestartPressed() const;

        float getSimSpeed() const;

        std::array<int, 3> getAnimalsToSpawn() const;

    private:
        sf::FloatRect area;
        SimulationConfig& config;
        static sf::Font font;

        bool runPressed = false;
        bool stopPressed = false;
        bool restartPressed = false;
        bool addAnimalsPressed = false;

        void initLayout();
        void initValues();

        std::vector<TextLabel> headers;
        std::vector<TextLabel> labels;
        std::vector<TextLabel> values;
        std::vector<TextLabel> editableLabels;
        std::vector<EditableLabel> editableValues;
        std::vector<Button> buttons;
        Slider speedSlider;

        // rabbits, maleWolves, femaleWolves
        std::array<int, 3> animalsToSpawn{ {0, 0, 0} };

        EditableLabel* focusedLabel;
};

