#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture textures[6];
    sf::Sprite sprites[6];
    float speeds[6] = { 0.3f, 0.5f, 0.8f, 1.f, 1.2, 1.5};
    float spriteWidth = 31470.0f;
    float spriteHeight = 2160.0f;
    float offsetX[6] = { 0, 0, 0, 0, 0, 0};

public:
    Background();
    bool loadTextures(const std::string& layer1, const std::string& layer2,
        const std::string& layer3, const std::string& layer4, const std::string& layer5, const std::string& layer6);
    void update(float playerX);
    void draw(sf::RenderWindow& window);
};

#endif
