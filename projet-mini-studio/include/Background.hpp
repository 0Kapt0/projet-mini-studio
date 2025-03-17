#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture textures[4];
    sf::Sprite sprites[4];
    float speeds[4] = { 0.05f, 0.0006f, 0.012f, 0.05f};
    float spriteWidth = 31470.0f;
    float spriteHeight = 2160.0f;
    float offsetX[4] = { 0, 0, 0, 0};

public:
    Background();
    bool loadTextures(const std::string& layer1, const std::string& layer2,
        const std::string& layer3, const std::string& layer4);
    void update(float playerX);
    void draw(sf::RenderWindow& window);
};

#endif
