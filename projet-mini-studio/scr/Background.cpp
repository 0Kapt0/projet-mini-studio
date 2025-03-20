#include "../include/Background.hpp"
#include <iostream>
#include <cmath>

Background::Background() {}

bool Background::loadTextures(const std::string& layer1,
    const std::string& layer2,
    const std::string& layer3,
    const std::string& layer4,
    const std::string& layer5,
    const std::string& layer6)
{
    std::string layers[6] = { layer1, layer2, layer3, layer4, layer5, layer6 };

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

void Background::update(float cameraX, bool followCamera) {
    for (int i = 0; i < 6; ++i) {
        // Si on veut suivre la caméra, on calcule le décalage en fonction de cameraX
        // Sinon, on fixe la position à zéro (ou toute autre position statique souhaitée)
        float posX = followCamera ? (cameraX - 960.f) : 0.f;  // Ajustez si nécessaire

        // On place le sprite verticalement (attention : dans votre code original 
        // vous aviez un "cameraX * 0, 1080" ce qui place le sprite à 1080px en Y.)
        // Si c'est l'effet voulu, on peut conserver "1080" :
        sprites[i].setPosition(posX, 1080.f);

        // Même chose pour le scrolling horizontal. S'il doit suivre la caméra, 
        // on calcule l'offset, sinon on peut fixer offset = 0.
        float offset = 0.f;
        if (followCamera) {
            offset = fmod(cameraX * speeds[i], spriteWidth);
            if (offset > 0) offset -= spriteWidth;
        }

        sprites[i].setTextureRect(sf::IntRect(static_cast<int>(offset), 0,
            static_cast<int>(spriteWidth),
            static_cast<int>(spriteHeight)));
    }
}

void Background::setSpeeds(const std::array<float, 6>& newSpeeds) {
    for (int i = 0; i < 6; ++i) {
        speeds[i] = newSpeeds[i];
    }
}


void Background::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 6; ++i) {
        window.draw(sprites[i]);
    }
}
