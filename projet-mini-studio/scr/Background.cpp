#include "../include/Background.hpp"
#include <iostream>
#include <cmath>

Background::Background() {}

bool Background::loadTextures(const std::string& layer1, const std::string& layer2,
    const std::string& layer3) {
    std::string layers[3] = { layer1, layer2, layer3};

    for (int i = 0; i < 3; ++i) {
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
    for (int i = 0; i < 3; ++i) {
        sprites[i].setPosition(0, 0);

        float offset = fmod(cameraX * speeds[i], 31470);
        if (offset > 0) offset -= 31470;

        sprites[i].setTextureRect(sf::IntRect(static_cast<int>(offset), 0, 31470, 2160));
    }
}

void Background::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 3; ++i) {
        window.draw(sprites[i]);
    }
}
