#include "../include/Foreground.hpp"
#include <iostream>
#include <cmath> // Pour fmod()

Foreground::Foreground() {}

// Charger les textures et configurer les sprites
bool Foreground::loadTextures(const std::string& layer1, const std::string& layer2) {
    std::string layers[2] = { layer1, layer2 };

    for (int i = 0; i < 2; ++i) {
        if (!textures[i].loadFromFile(layers[i])) {
            std::cerr << "Erreur de chargement de la texture : " << layers[i] << std::endl;
            return false;
        }

        // Activer la répétition de la texture
        textures[i].setRepeated(true);

        // Configurer le sprite avec la texture
        sprites[i].setTexture(textures[i]);

        // Ajuster l'échelle pour remplir l'écran
        sprites[i].setScale(1920.0f / textures[i].getSize().x + 0.4f, 1080.0f / textures[i].getSize().y + 0.4f);
    }

    return true;
}

// Mettre à jour la position du foreground pour suivre la caméra
void Foreground::update(float cameraX) {
    for (int i = 0; i < 2; ++i) {
        // Fixer la position X du sprite au centre de la caméra
        sprites[i].setPosition(cameraX - 960, -300); // Centré sur la caméra

        // Défilement de la texture à l'intérieur du sprite
        float offset = fmod(cameraX * speeds[i] * 2.0f, 1920);
        if (offset > 0) offset -= 1920;

        sprites[i].setTextureRect(sf::IntRect(static_cast<int>(offset), 0, 1920, 1080));
    }
}

// Dessiner le foreground
void Foreground::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 2; ++i) {
        window.draw(sprites[i]);
    }
}
