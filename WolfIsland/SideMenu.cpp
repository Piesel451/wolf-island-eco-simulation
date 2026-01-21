#include "SideMenu.h"
#include "Animal.h"
#include "ResourceLoader.h"
#include "resource.h"
#include <iostream>

sf::Font SideMenu::font;

SideMenu::SideMenu(sf::FloatRect area, SimulationConfig& config) : area(area), config(config)
{
    initLayout();
}

bool SideMenu::loadFont() {
    bool fontLoaded = loadFromResource(font, IDR_FONT_ARIAL, L"TTF");
	return fontLoaded;
}

sf::Font& SideMenu::getFont() {
    return font;
}

//dodaje pola sf::Text do wektorów dynamicLabels i staticLabels
void SideMenu::initLayout() {

    const float left = area.position.x + 20.f;
    const float top = area.position.y + 10.f;
    const float lineHeight = 22.f;
    const float blockSpacing = 26.f;

    headers.emplace_back(sf::Vector2f(left, top + 0.f * lineHeight), "STATYSTYKI ZWIERZAT", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 1.f * lineHeight), "Zywe kroliki: ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 2.f * lineHeight), "Zywe samce wilka: ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 3.f * lineHeight), "Zywe samice wilka: ", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 5.f * lineHeight), "POCZATKOWA ILOSC ZWIERZAT", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 6.f * lineHeight), "Kroliki: ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 7.f * lineHeight), "Wilki (samce): ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 8.f * lineHeight), "Wilki (samice): ", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 10.f * lineHeight), "PARAMETRY ZWIERZAT", font, 23);

    headers.emplace_back(sf::Vector2f(left, top + 12.f * lineHeight), "KROLIK: ", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 13.f * lineHeight), "Startowa energia (K): ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 14.f * lineHeight), "Max energia (K): ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 15.f * lineHeight), "Strata energii (K): ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 16.f * lineHeight), "Prawd. rozmnazania (K): ", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 18.f * lineHeight), "WILK: ", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 19.f * lineHeight), "Startowa energia (W): ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 20.f * lineHeight), "Max energia (K): ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 21.f * lineHeight), "Max energia (W): ", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 22.f * lineHeight), "Zysk energii (W): ", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 24.f * lineHeight), "PREDKOSC SYMULACJI: ", font, 23);
    speedSlider = Slider(sf::Vector2f(left + 30, top + 26.f * lineHeight), 400);
    
    buttons.emplace_back(sf::Vector2f(left + 30, top + 27.f * lineHeight), sf::Vector2f(100.0f, 50.0f), "Start", font, 18);
    buttons.emplace_back(sf::Vector2f(left + 180, top + 27.f * lineHeight), sf::Vector2f(100.0f, 50.0f), "Stop", font, 18);
    buttons.emplace_back(sf::Vector2f(left + 330, top + 27.f * lineHeight), sf::Vector2f(100.0f, 50.0f), "Restart", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 30.f * lineHeight), "DODAJ ZWIERZETA: ", font, 23);
    editableLabels.emplace_back(sf::Vector2f(left, top + 31.f * lineHeight), "Kroliki: ", font, 18);
    editableLabels.emplace_back(sf::Vector2f(left, top + 32.f * lineHeight), "Wilki(samce): ", font, 18);
    editableLabels.emplace_back(sf::Vector2f(left, top + 33.f * lineHeight), "Wilki(samice): ", font, 18);
    buttons.emplace_back(sf::Vector2f(left, top + 34.f * lineHeight), sf::Vector2f(100.0f, 50.0f), "Dodaj", font, 18);

    const float valX = left + 320.f;
    values.reserve(labels.size());
    editableValues.reserve(editableLabels.size());

    for (int i = 0; i < labels.size(); i++) {
        const sf::Vector2f pos = labels[i].getPosition();
        values.emplace_back(sf::Vector2f(valX, pos.y), "0", font, 18);
    }

    for (int i = 0; i < editableLabels.size(); i++) {
        const sf::Vector2f pos = editableLabels[i].getPosition();
        editableValues.emplace_back(sf::Vector2f(valX, pos.y), "0", font, 18);
    }
    initValues();

}

void SideMenu::initValues() {
    values[0].setString(std::to_string(config.rabbitCount));
    values[1].setString(std::to_string(config.maleWolvesCount));
    values[2].setString(std::to_string(config.femaleWolvesCount));

    values[3].setString(std::to_string(config.rabbitCount));
    values[4].setString(std::to_string(config.maleWolvesCount));
    values[5].setString(std::to_string(config.femaleWolvesCount));

    values[6].setString(std::to_string(static_cast<int>(config.rabbitStartingEnergy)));
    values[7].setString(std::to_string(static_cast<int>(config.rabbitMaxEnergy)));
    values[8].setString(std::to_string(static_cast<int>(config.rabbitEnergyLoss)));
    values[9].setString(std::to_string(config.rabbitReproduceProb));

    values[10].setString(std::to_string(static_cast<int>(config.wolfStartingEnergy)));
    values[11].setString(std::to_string(static_cast<int>(config.wolfMaxEnergy)));
    values[12].setString(std::to_string(static_cast<int>(config.wolfEnergyGain)));
    values[13].setString(std::to_string(static_cast<int>(config.wolfEnergyLoss)));

    speedSlider.setValue(config.simulationSpeed);
}

void SideMenu::processEvent(const sf::Event& event, sf::Window& window) {
    runPressed = false;
    stopPressed = false;
    restartPressed = false;
    addAnimalsPressed = false;

    //obsluga focusa na editableValue
    if (event.is<sf::Event::MouseButtonPressed>() && event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2f mousePos(sf::Mouse::getPosition(window));

        //zdjecie focusa ze starego
        if (focusedLabel)
            focusedLabel->setFocused(false);

        focusedLabel = nullptr;

        //znajdz nowy focus
        for (auto& value : editableValues) {
            if (value.hitTest(mousePos)) {
                focusedLabel = &value;
                value.setFocused(true);
                break;
            }
        }
    }
    for (auto& value : editableValues) {
        value.handleEvent(event);
    }

    //obsluga przyciskow
    for (int i = 0; i < buttons.size(); i++){
        auto& button = buttons[i];

        button.processEvent(event, window);

        if (button.isClicked()) {
            if (i == 0) {
                //start symulacji
                runPressed = true;
                return;
            }
            else if (i == 1) {
                //stop symulacji
                stopPressed = true;
                return;
            }
            else if (i == 2) {
                //restart symulacji
                restartPressed = true;
                return;
            }
            else if (i == 3) {
                //dodanie zwierzat
                addAnimalsPressed = true;
                return;
            }
        }
    }

    speedSlider.handleEvent(event, window);

}

void SideMenu::update(std::vector<std::unique_ptr<Animal>>& animals) {
    if (values.size() < 14)
        return;

    int rabbitsAlive = 0;
    int maleWolvesAlive = 0;
    int femaleWolvesAlive = 0;

    for (const auto& a : animals)
    {
        if (!a || !a->isAlive())
            continue;

        switch (a->getType())
        {
        case AnimalType::Rabbit:
            rabbitsAlive++;
            break;
        case AnimalType::Wolf:
            if (a->getSex() == 0)
                femaleWolvesAlive++;
            else
                maleWolvesAlive++;
            break;
        default:
            break;
        }
    }

    values[0].setString(std::to_string(rabbitsAlive));
    values[1].setString(std::to_string(maleWolvesAlive));
    values[2].setString(std::to_string(femaleWolvesAlive));
}

void SideMenu::draw(sf::RenderWindow& window) const {
    for (const auto& button : buttons)
        button.draw(window);

    for (const auto& header : headers)
        header.draw(window);

    for (const auto& label : labels)
        label.draw(window);

	for (const auto& value : values)
        value.draw(window);

	for (const auto& editableLbl : editableLabels)
        editableLbl.draw(window);

	for (const auto& editableVal : editableValues)
        editableVal.draw(window);

    speedSlider.draw(window);
}

bool SideMenu::isRunPressed() const {
	return runPressed;
}

bool SideMenu::isStopPressed() const {
	return stopPressed;
}

bool SideMenu::isAddAnimalsPressed() const {
    return addAnimalsPressed;
}

bool SideMenu::isRestartPressed() const {
    return restartPressed;
}

float SideMenu::getSimSpeed() const {
    return speedSlider.getValue();
}

std::array<int, 3> SideMenu::getAnimalsToSpawn() const
{
    std::array<int, 3> result{ {0, 0, 0} };

    auto parseInt = [](const std::string& s) -> int {
        try {
            return std::stoi(s);
        }
        catch (...) {
            return 0;
        }
    };

    if (editableValues.size() == 3) {
        result[0] = parseInt(editableValues[0].getString()); // króliki
        result[1] = parseInt(editableValues[1].getString()); // wilki M
        result[2] = parseInt(editableValues[2].getString()); // wilki F
    }

    return result;
}

