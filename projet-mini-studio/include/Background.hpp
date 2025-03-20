#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <string>

class Background {
private:
    sf::Texture textures[6];
    sf::Sprite sprites[6];
    float speeds[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
    float spriteWidth = 32715.0f;
    float spriteHeight = 2160.0f;

public:
    Background();
    bool loadTextures(const std::string& layer1,
        const std::string& layer2,
        const std::string& layer3,
        const std::string& layer4,
        const std::string& layer5,
        const std::string& layer6);

    // Nouvelle méthode pour changer les vitesses des couches
    void setSpeeds(const std::array<float, 6>& newSpeeds);

    // On peut ajouter un bool "followCamera" si tu veux activer/désactiver le suivi
    void update(float cameraX , bool followCamera = true);

    void draw(sf::RenderWindow& window);
};

#endif
