#include "../include/Background.hpp"
#include <iostream>
#include <cmath>

Background::Background() {}

bool Background::loadTextures(const string& layer1,
    const string& layer2,
    const string& layer3,
    const string& layer4,
    const string& layer5,
    const string& layer6)
{
    string layers[6] = { layer1, layer2, layer3, layer4, layer5, layer6 };

    for (int i = 0; i < 6; ++i) {
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

void Background::update(float cameraX, bool followCamera) {
    for (int i = 0; i < 6; ++i) {

        float rawPosX = (followCamera ? (cameraX - 960.f) : 0.f);
        int posX = static_cast<int>(floor(rawPosX));

        sprites[i].setPosition(static_cast<float>(posX), 1080.f);

        float rawOffset = (followCamera ? cameraX * speeds[i] : 0.f);

        int offsetInt = static_cast<int>(floor(rawOffset)) % static_cast<int>(spriteWidth);

        if (offsetInt > 0) offsetInt -= static_cast<int>(spriteWidth);

        sprites[i].setTextureRect(
            IntRect(offsetInt,
                0, 
                static_cast<int>(spriteWidth),
                static_cast<int>(spriteHeight))
        );
    }
}

void Background::setSpeeds(const array<float, 6>& newSpeeds) {
    for (int i = 0; i < 6; ++i) {
        speeds[i] = newSpeeds[i];
    }
}

void Background::draw(RenderWindow& window) {
    for (int i = 0; i < 6; ++i) {
        window.draw(sprites[i]);
    }
}
