#ifndef ENEMY_SELECTOR_HPP
#define ENEMY_SELECTOR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Votre enum, par exemple :
enum class EnemyType {
    None,
    Flying,
    Ranged,
    Basic,
    ChargingBoss,
    // ...
};

class EnemySelector {
public:
    EnemySelector(const std::string& texturePath, int iconSize);

    // Gère le clic (comme le TileSelector)
    void handleEvent(sf::Event event, sf::RenderWindow& window);

    // Dessine la palette (comme TileSelector)
    void draw(sf::RenderWindow& window);

    // Récupère l'ennemi sélectionné
    EnemyType getSelectedEnemy() const;

private:
    sf::Texture m_texture;
    std::vector<sf::Sprite> m_sprites;  // Toutes les icônes
    int m_iconSize;

    // Indice choisi
    int m_selectedIndex;

    // Type d’ennemi choisi
    EnemyType m_selectedEnemy;

    // Un tableau qui mappe l'index -> EnemyType (comme tile index -> tile ID).
    // L’ordre dans m_sprites et m_enemyTypes doit correspondre
    std::vector<EnemyType> m_enemyTypes;
};

#endif
