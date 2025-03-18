#include "../include/EnemySelector.hpp"
#include <iostream>

EnemySelector::EnemySelector(const std::string& texturePath, int iconSize)
    : m_iconSize(iconSize)
    , m_selectedIndex(-1)
    , m_selectedEnemy(EnemyType::None)
{
    // Charger la texture
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur lors du chargement du EnemySelector : " << texturePath << std::endl;
    }

    // Calculer combien d'icônes en X/Y
    int sheetWidth = m_texture.getSize().x;
    int sheetHeight = m_texture.getSize().y;

    int iconsPerRow = sheetWidth / iconSize;
    int iconsRows = sheetHeight / iconSize;

    // On prépare un tableau de EnemyType correspondant aux indices
    // Par exemple, 4 ennemis
    m_enemyTypes = {
        EnemyType::Flying,
        EnemyType::Ranged,
        EnemyType::Basic,
        EnemyType::ChargingBoss
        // Ajoutez si vous en avez plus
    };

    // Remplir le vector de sprites
    for (int y = 0; y < iconsRows; ++y) {
        for (int x = 0; x < iconsPerRow; ++x) {
            int index = y * iconsPerRow + x;
            if (index >= (int)m_enemyTypes.size()) {
                break; // plus d'icônes que d'EnemyType => on stop
            }

            sf::Sprite sprite;
            sprite.setTexture(m_texture);
            sprite.setTextureRect(sf::IntRect(x * iconSize, y * iconSize, iconSize, iconSize));
            sprite.setPosition(float(x * iconSize), float(y * iconSize));
            m_sprites.push_back(sprite);
        }
    }
}

void EnemySelector::handleEvent(sf::Event event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Récup position souris en pixels (vue par défaut)
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Calculer la zone couverte par la palette
            int sheetWidth = m_texture.getSize().x;
            int sheetHeight = m_texture.getSize().y;

            if (mousePos.x >= 0 && mousePos.x < sheetWidth &&
                mousePos.y >= 0 && mousePos.y < sheetHeight)
            {
                // On clique dans la zone palette
                int tileX = mousePos.x / m_iconSize;
                int tileY = mousePos.y / m_iconSize;

                int iconsPerRow = sheetWidth / m_iconSize;
                int index = tileY * iconsPerRow + tileX;

                if (index >= 0 && index < (int)m_sprites.size()) {
                    m_selectedIndex = index;
                    m_selectedEnemy = m_enemyTypes[index];
                    std::cout << "[EnemySelector] Ennemi choisi index=" << index << std::endl;
                }
            }
        }
    }
}

void EnemySelector::draw(sf::RenderWindow& window)
{
    // Sauvegarde et passage en defaultView (comme TileSelector)
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    // Dessiner chaque icône
    for (auto& sprite : m_sprites) {
        window.draw(sprite);
    }

    // Dessiner outline si un index est sélectionné
    if (m_selectedIndex >= 0 && m_selectedIndex < (int)m_sprites.size()) {
        sf::Vector2f pos = m_sprites[m_selectedIndex].getPosition();
        sf::RectangleShape outline(sf::Vector2f((float)m_iconSize, (float)m_iconSize));
        outline.setPosition(pos);
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color::Green);
        outline.setOutlineThickness(2.f);
        window.draw(outline);
    }

    // On restore la vue
    window.setView(oldView);
}

EnemyType EnemySelector::getSelectedEnemy() const {
    return m_selectedEnemy;
}
