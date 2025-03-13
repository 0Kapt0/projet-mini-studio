#include "../include/Background.hpp"
#include <iostream>
#include <cmath>

Background::Background() {}

bool Background::loadTextures(const std::string& layer1, const std::string& layer2,
    const std::string& layer3, const std::string& layer4) {
    std::string layers[4] = { layer1, layer2, layer3, layer4 };

    for (int i = 0; i < 4; ++i) {
        if (!textures[i].loadFromFile(layers[i])) {
            std::cerr << "Erreur de chargement de la texture : " << layers[i] << std::endl;
            return false;
        }

        textures[i].setRepeated(true);

        sprites[i].setTexture(textures[i]);

        sprites[i].setScale(1920.0f / textures[i].getSize().x + 0.4f, 1080.0f / textures[i].getSize().y + 0.4f);
    }

    return true;
}

void Background::update(float cameraX) {
    for (int i = 0; i < 4; ++i) {
        sprites[i].setPosition(cameraX - 960, -300);

        float offset = fmod(cameraX * speeds[i], 1920);
        if (offset > 0) offset -= 1920;

        sprites[i].setTextureRect(sf::IntRect(static_cast<int>(offset), 0, 1920, 1080));
    }
}

void Background::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 4; ++i) {
        window.draw(sprites[i]);
    }
}
