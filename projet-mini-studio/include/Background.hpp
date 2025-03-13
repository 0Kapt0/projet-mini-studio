#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>

class Background {
private:
    sf::Texture textures[4];   // Textures pour chaque couche
    sf::Sprite sprites[4];     // Un seul sprite par couche
    float speeds[4] = { 0.01f, 0.03f, 0.06f, 0.10f }; // Vitesse du défilement
    float spriteWidth = 1920.0f;  // Largeur de l'écran
    float spriteHeight = 1080.0f; // Hauteur de l'écran
    float offsetX[4] = { 0, 0, 0, 0 }; // Défilement horizontal de chaque couche

public:
    Background();
    bool loadTextures(const std::string& layer1, const std::string& layer2,
        const std::string& layer3, const std::string& layer4);
    void update(float playerX);
    void draw(sf::RenderWindow& window);
};

#endif
