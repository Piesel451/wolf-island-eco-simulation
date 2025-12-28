#include "ConfigMenu.h"
#include <iostream>

ConfigMenu::ConfigMenu(sf::RenderWindow& window, SimulationConfig& config) : window(window), config(config)
{
    initLayout();
}

bool ConfigMenu::run() {
    bool done = false;

    while (window.isOpen() && !done)
    {
        handleEvents(done);

        window.clear(sf::Color(30, 30, 50));

        for (const auto& hdr : headers) {
            hdr.draw(window);
        }
        for (const auto& lbl : labels) {
            lbl.draw(window);
        }
        for (const auto& val : values) {
            val.draw(window);
        }

        for (const auto& btn : buttons)
            btn.draw(window);

        window.display();
    }

    if (done) {
        //zmiana kofiga przekazanego w refencji
        if (values.size() >= 14) {
            auto toInt = [](const sf::String& s) {
                return std::stoi(s.toAnsiString());
            };

            auto toFloat = [](const sf::String& s) {
                return std::stof(s.toAnsiString());
            };

            config.rows = toInt(values[0].getString());
            config.cols = toInt(values[1].getString());
            config.rabbitCount = toInt(values[2].getString());
            config.maleWolvesCount = toInt(values[3].getString());
            config.femaleWolvesCount = toInt(values[4].getString());

            config.rabbitStartingEnergy = toFloat(values[5].getString());
            config.rabbitMaxEnergy = toFloat(values[6].getString());
            config.rabbitReproduceProb = toFloat(values[7].getString());
            config.rabbitEnergyLoss = toFloat(values[8].getString());

            config.wolfStartingEnergy = toFloat(values[9].getString());
            config.wolfMaxEnergy = toFloat(values[10].getString());
            config.wolfEnergyGain = toFloat(values[11].getString());
            config.wolfEnergyLoss = toFloat(values[12].getString());

            config.simulationSpeed = toFloat(values[13].getString());
        }
    }

    return done;
}

void ConfigMenu::initLayout() {
    const float left = 40.f;
    const float top = 40.f;
    const float lh = 26.f;

    labels.clear();
    headers.clear();
    values.clear();
    buttons.clear();

    const sf::Font& font = SideMenu::getFont();

    // etykiety
    headers.emplace_back(sf::Vector2f(left, top + 0.f * lh),  "MAPA", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 1.f * lh),  "Wiersze (rows):", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 2.f * lh),  "Kolumny (cols):", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 4.f * lh),  "ZWIERZETA NA START", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 5.f * lh),  "Kroliki (start):", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 6.f * lh),  "Wilki samce (start):", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 7.f * lh),  "Wilki samice (start):", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 9.f * lh),  "PARAMETRY KROLIKA", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 10.f * lh), "Krolik energia start:", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 11.f * lh), "Krolik energia max:", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 12.f * lh), "Krolik prawd. rozm.:", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 13.f * lh), "Krolik strata energii:", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 15.f * lh), "PARAMETRY WILKA", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 16.f * lh), "Wilk energia start:", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 17.f * lh), "Wilk energia max:", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 18.f * lh), "Wilk zysk energii:", font, 18);
    labels.emplace_back(sf::Vector2f(left, top + 19.f * lh), "Wilk strata energii:", font, 18);

    headers.emplace_back(sf::Vector2f(left, top + 21.f * lh), "SYMULACJA", font, 23);
    labels.emplace_back(sf::Vector2f(left, top + 22.f * lh), "Predkosc symulacji:", font, 18);

    // pola z wartosciami
    const float valX = left + 320.f;
    values.reserve(labels.size());

    for (int i = 0; i < labels.size(); i++)
    {
        // Zak³adam, ¿e TextLabel udostêpnia dostêp do sf::Text przez pole label
        const sf::Vector2f pos = labels[i].getPosition();
        values.emplace_back(sf::Vector2f(valX, pos.y), "---", font, 18);
    }
    initValues();

    // przycisk "potwierdz"
    const float btnY = top + 24.f * lh;
    buttons.emplace_back(
        sf::Vector2f(left, btnY),
        sf::Vector2f(140.f, 40.f),
        "Potwierdz",
        font,
        18
    );
}

void ConfigMenu::initValues() {
    values[0].setString(std::to_string(config.rows));
    values[1].setString(std::to_string(config.cols));
    values[2].setString(std::to_string(config.rabbitCount));
    values[3].setString(std::to_string(config.maleWolvesCount));
    values[4].setString(std::to_string(config.femaleWolvesCount));

    values[5].setString(std::to_string(static_cast<int>(config.rabbitStartingEnergy)));
    values[6].setString(std::to_string(static_cast<int>(config.rabbitMaxEnergy)));
    values[7].setString(std::to_string(config.rabbitReproduceProb));
    values[8].setString(std::to_string(static_cast<int>(config.rabbitEnergyLoss)));

    values[9].setString(std::to_string(static_cast<int>(config.wolfStartingEnergy)));
    values[10].setString(std::to_string(static_cast<int>(config.wolfMaxEnergy)));
    values[11].setString(std::to_string(static_cast<int>(config.wolfEnergyGain)));
    values[12].setString(std::to_string(static_cast<int>(config.wolfEnergyLoss)));

    values[13].setString(std::to_string(config.simulationSpeed));
}

void ConfigMenu::changeValue(int delta) { // TODO: obsluga zmiany wartosci
    (void)delta;
}

void ConfigMenu::handleEvents(bool& done) {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(sf::Mouse::getPosition(window));

            //zdjecie focusa ze starego
            if (focusedLabel)
                focusedLabel->setFocused(false);

            focusedLabel = nullptr;

            //znajdz nowy focus
            for (auto& value : values) {
                if (value.hitTest(mousePos)) {
                    focusedLabel = &value;
                    value.setFocused(true);
                    break;
                }
            }
        }

        for (auto& value : values) {
            value.handleEvent(*event);
        }

        for (int i = 0; i < buttons.size(); i++) {
            auto& button = buttons[i];
            button.processEvent(*event, window);
            if (button.isClicked()) {
                std::cout << "confirm pressed\n";
                done = true;
            }
        }
    }
}
