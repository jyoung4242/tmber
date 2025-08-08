#pragma once
#include <SFML/Graphics.hpp>
#include <string>

std::optional<sf::Sprite> loadSpriteFromPath(const std::string& path, sf::Vector2f pos, sf::Texture& textureOut);

class Logger {
public:
    static void info(const std::string& message);

    static void warn(const std::string& message);

    static void error(const std::string& message);

private:
    static void log(const std::string& level, const std::string& message);
};

void centerTextBox(sf::Text& box);