#ifndef FOREGROUND_HPP
#define FOREGROUND_HPP

#include <SFML/Graphics.hpp>

class Foreground {
private:
    sf::Texture textures[2];   // Textures pour chaque couche du foreground
    sf::Sprite sprites[2];     // Un seul sprite par couche
    float speeds[2] = { 0.15f, 0.20f}; // Vitesse du défilement (foreground plus rapide)
    float spriteWidth = 1920.0f;  // Largeur de l'écran
    float spriteHeight = 1080.0f; // Hauteur de l'écran
    float offsetX[2] = { 0, 0}; // Défilement horizontal de chaque couche

public:
    Foreground();
    bool loadTextures(const std::string& layer1, const std::string& layer2);
    void update(float playerX);
    void draw(sf::RenderWindow& window);
};

#endif

