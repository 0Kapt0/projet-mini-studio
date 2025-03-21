#include "../include/Foreground.hpp"
#include <iostream>
#include <cmath>

Foreground::Foreground() {}

bool Foreground::loadTextures(const string& layer1, const string& layer2) {
    string layers[2] = { layer1, layer2};

    for (int i = 0; i < 2; ++i) {
        if (!textures[i].loadFromFile(layers[i])) {
            cerr << "Erreur de chargement de la texture : " << layers[i] << endl;
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

        float offset = fmod(cameraX * speeds[i] * 2.0f, 32715);
        if (offset > 0) offset -= 32715;

        sprites[i].setTextureRect(IntRect(static_cast<int>(offset), 0, 32715, 4320));
    }
}

void Foreground::draw(RenderWindow& window) {
    for (int i = 0; i < 2; ++i) {
        window.draw(sprites[i]);
    }
}
