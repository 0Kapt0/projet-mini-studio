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

        // On répète la texture pour le scrolling
        textures[i].setRepeated(true);

        // Affectation au sprite
        sprites[i].setTexture(textures[i]);

        // Mise à l'échelle (si nécessaire)
        sprites[i].setScale(0.5f, 0.5f);
    }

    return true;
}

void Background::update(float cameraX, bool followCamera) {
    for (int i = 0; i < 6; ++i) {

        // --- 1) Calcul de la position en X ---

        float rawPosX = (followCamera ? (cameraX - 960.f) : 0.f);
        // Pour éviter le subpixel rendering, on arrondit la position sur un entier
        int posX = static_cast<int>(std::floor(rawPosX));

        // On fixe la position verticale ; ici vous utilisez 1080.f d'après le code original.
        // Ajustez si nécessaire (par ex. 0.f si vous voulez le background tout en haut).
        sprites[i].setPosition(static_cast<float>(posX), 1080.f);

        // --- 2) Calcul du scrolling dans la texture ---
        // Si on ne suit pas la caméra, offset = 0
        float rawOffset = (followCamera ? cameraX * speeds[i] : 0.f);

        // On cast en entier pour forcer l'alignement, puis modulo la largeur de la texture
        int offsetInt = static_cast<int>(std::floor(rawOffset)) % static_cast<int>(spriteWidth);

        // Dans le cas où offsetInt est positif, on le réajuste
        if (offsetInt > 0) offsetInt -= static_cast<int>(spriteWidth);

        // --- 3) On applique ce rect à la texture du sprite ---
        // Notez qu’on utilise spriteWidth / spriteHeight en tant que largeur/hauteur initiales
        // de la texture (vous pouvez adapter si votre texture ne correspond pas exactement à ces valeurs)
        sprites[i].setTextureRect(
            sf::IntRect(offsetInt,               // décalage horizontal
                0,                       // décalage vertical
                static_cast<int>(spriteWidth),   // largeur
                static_cast<int>(spriteHeight))   // hauteur
        );
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
