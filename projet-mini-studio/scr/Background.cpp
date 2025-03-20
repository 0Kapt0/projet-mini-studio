#include "../include/Background.hpp"
#include <iostream>
#include <cmath>

Background::Background() {}

bool Background::loadTextures(const std::string& layer1, const std::string& layer2,
    const std::string& layer3, const std::string& layer4, const std::string& layer5, const std::string& layer6) {
    std::string layers[6] = { layer1, layer2, layer3, layer4, layer5 };

    for (int i = 0; i < 6; ++i) {
        if (!textures[i].loadFromFile(layers[i])) {
            std::cerr << "Erreur de chargement de la texture : " << layers[i] << std::endl;
            return false;
        }

        textures[i].setRepeated(true);

        sprites[i].setTexture(textures[i]);

        sprites[i].setScale(0.5f, 0.5f);
    }

    return true;
}

void Background::update(float cameraX) {
    for (int i = 0; i < 6; ++i) {
        sprites[i].setPosition(cameraX - 960,1080);

        float offset = fmod(cameraX * speeds[i], 31470);
        if (offset > 0) offset -= 31470;

        sprites[i].setTextureRect(sf::IntRect(static_cast<int>(offset), 0, 31470, 2160));
    }
}

void Background::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 6; ++i) {
        window.draw(sprites[i]);
    }
}
