#include "../include/Background.hpp"
#include <iostream>
#include <cmath> // Pour fmod()

Background::Background() {}

// Charger les textures et configurer les sprites
bool Background::loadTextures(const std::string& layer1, const std::string& layer2,
    const std::string& layer3, const std::string& layer4) {
    std::string layers[4] = { layer1, layer2, layer3, layer4 };

    for (int i = 0; i < 4; ++i) {
        if (!textures[i].loadFromFile(layers[i])) {
            std::cerr << "Erreur de chargement de la texture : " << layers[i] << std::endl;
            return false;
        }

        // Activer la r�p�tition de la texture
        textures[i].setRepeated(true);

        // Configurer le sprite avec la texture
        sprites[i].setTexture(textures[i]);

        // Ajuster l'�chelle pour remplir l'�cran
        sprites[i].setScale(1920.0f / textures[i].getSize().x + 0.4f, 1080.0f / textures[i].getSize().y + 0.4f);
    }

    return true;
}

// Mettre � jour la position du background pour suivre la cam�ra
void Background::update(float cameraX) {
    for (int i = 0; i < 4; ++i) {
        // Fixer la position X du sprite au centre de la cam�ra
        sprites[i].setPosition(cameraX - 960, -300); // Centr� sur la cam�ra

        // D�filement de la texture � l'int�rieur du sprite
        float offset = fmod(cameraX * speeds[i], 1920);
        if (offset > 0) offset -= 1920;

        sprites[i].setTextureRect(sf::IntRect(static_cast<int>(offset), 0, 1920, 1080));
    }
}

// Dessiner le background
void Background::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 4; ++i) {
        window.draw(sprites[i]);
    }
}
