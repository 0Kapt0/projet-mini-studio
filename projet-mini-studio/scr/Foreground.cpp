#include "../include/Foreground.hpp"
#include <iostream>
#include <cmath>

Foreground::Foreground() {}

bool Foreground::loadTextures(const std::string& layer1, const std::string& layer2) {
    std::string layers[2] = { layer1, layer2};

    for (int i = 0; i < 2; ++i) {
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

void Foreground::update(float cameraX) {
    for (int i = 0; i < 2; ++i) {
        sprites[i].setPosition(0,0);

        float offset = fmod(cameraX * speeds[i] * 2.0f, 31470);
        if (offset > 0) offset -= 31470;

        sprites[i].setTextureRect(sf::IntRect(static_cast<int>(offset), 0, 31470, 2160));
    }
}

void Foreground::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 2; ++i) {
        window.draw(sprites[i]);
    }
}
