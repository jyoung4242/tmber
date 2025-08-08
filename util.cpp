
#include <SFML/Graphics.hpp>
#include "util.h"
#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>  

std::optional<sf::Sprite> loadSpriteFromPath(const std::string& path,sf::Vector2f pos, sf::Texture& textureOut) {
    if (!textureOut.loadFromFile(path)) return std::nullopt;

    sf::Sprite sprite(textureOut);
    sprite.setPosition(pos);
    return sprite;
}

void Logger::info(const std::string& message) {
    log("INFO", message);
}

void Logger::warn(const std::string& message) {
    log("WARN", message);
}

void Logger::error(const std::string& message) {
    log("ERROR", message);
}

void Logger::log(const std::string& level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm;
    localtime_s(&tm, &t);

    std::cout << "[" << std::put_time(&tm, "%H:%M:%S") << "] "
        << "[" << level << "] " << message << std::endl;
}

void centerTextBox(sf::Text& box) {
    sf::FloatRect textRec = box.getLocalBounds();
    box.setOrigin(sf::Vector2f(textRec.position.x + textRec.size.x/2.0,textRec.position.y + textRec.size.y/2.0));
    box.setPosition(sf::Vector2f(1920/2,1080/2.0));
}