#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture textures[3];
    sf::Sprite sprites[3];
    float speeds[3] = { 0.01f, 0.03f, 0.06f};
    float spriteWidth = 31470.0f;
    float spriteHeight = 2160.0f;
    float offsetX[3] = { 0, 0, 0};

public:
    Background();
    bool loadTextures(const std::string& layer1, const std::string& layer2,
        const std::string& layer3);
    void update(float playerX);
    void draw(sf::RenderWindow& window);
};

#endif
