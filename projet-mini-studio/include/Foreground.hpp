#ifndef FOREGROUND_HPP
#define FOREGROUND_HPP

#include <SFML/Graphics.hpp>

class Foreground {
private:
    sf::Texture textures[2];
    sf::Sprite sprites[2];
    float speeds[2] = { 0.f};
    float spriteWidth = 32715.0f;
    float spriteHeight = 4320.0f;
    float offsetX[2] = { 0};

public:
    Foreground();
    bool loadTextures(const std::string& layer1, const std::string& layer2);
    void update(float playerX);
    void draw(sf::RenderWindow& window);
};

#endif

