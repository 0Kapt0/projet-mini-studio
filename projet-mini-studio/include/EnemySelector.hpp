#ifndef ENEMY_SELECTOR_HPP
#define ENEMY_SELECTOR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

enum class EnemyType {
    Flying,
    Ranged,
    Basic,
    ChargingBoss,
    // etc.
};

class EnemySelector {
public:
    EnemySelector(const std::string& texturePath, int iconSize);

    // G�re le clic souris pour s�lectionner un type d�ennemi
    void handleEvent(sf::Event event, sf::RenderWindow& window);

    // Dessine la palette d�ennemis (fixe, en haut � gauche)
    void draw(sf::RenderWindow& window);

    // Renvoie le type d�ennemi s�lectionn� (ou "None" si rien)
    EnemyType getSelectedEnemy() const { return m_selectedEnemy; }

private:
    sf::Texture m_texture;
    std::vector<sf::Sprite> m_enemyIcons;
    int m_iconSize;

    // L�ordre des ic�nes correspond aux indices [0..n-1]
    // On peut faire un mapping index->EnemyType
    std::vector<EnemyType> m_enemyTypes;

    EnemyType m_selectedEnemy;
    int m_selectedIndex;
};

#endif
