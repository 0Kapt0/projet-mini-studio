#include "../include/EnemySelector.hpp"
#include <iostream>

EnemySelector::EnemySelector(const string& texturePath, int iconSize)
    : m_iconSize(iconSize)
    , m_selectedIndex(-1)
    , m_selectedEnemy(EnemyType::None)
{
    if (!m_texture.loadFromFile(texturePath)) {
        cerr << "Erreur lors du chargement du EnemySelector : " << texturePath << endl;
    }

    int sheetWidth = m_texture.getSize().x;
    int sheetHeight = m_texture.getSize().y;

    int iconsPerRow = sheetWidth / iconSize;
    int iconsRows = sheetHeight / iconSize;

    m_enemyTypes = {
        EnemyType::Flying,
        EnemyType::Ranged,
        EnemyType::Basic,
        EnemyType::ChargingBoss
    };

    int offsetX = 900;

    for (int y = 0; y < iconsRows; ++y) {
        for (int x = 0; x < iconsPerRow; ++x) {
            int index = y * iconsPerRow + x;
            if (index >= (int)m_enemyTypes.size()) {
                break;
            }

            Sprite sprite;
            sprite.setTexture(m_texture);
            sprite.setTextureRect(IntRect(x * iconSize, y * iconSize, iconSize, iconSize));
            sprite.setPosition(float(x * iconSize + offsetX), float(y * iconSize));
            m_sprites.push_back(sprite);
        }
    }
}

void EnemySelector::handleEvent(Event event, RenderWindow& window)
{
    int offsetX = 900;
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);

            int sheetWidth = m_texture.getSize().x;
            int sheetHeight = m_texture.getSize().y;

            if (mousePos.x >= offsetX && mousePos.x < offsetX + sheetWidth &&
                mousePos.y >= 0 && mousePos.y < sheetHeight)
            {
                int tileX = (mousePos.x - offsetX) / m_iconSize;
                int tileY = mousePos.y / m_iconSize;

                int iconsPerRow = sheetWidth / m_iconSize;
                int index = tileY * iconsPerRow + tileX;

                if (index >= 0 && index < (int)m_sprites.size()) {
                    m_selectedIndex = index;
                    m_selectedEnemy = m_enemyTypes[index];
                    cout << "[EnemySelector] Ennemi choisi index=" << index << endl;
                }
            }
        }
    }
}

void EnemySelector::draw(RenderWindow& window)
{
    View oldView = window.getView();
    window.setView(window.getDefaultView());

    for (auto& sprite : m_sprites) {
        window.draw(sprite);
    }

    if (m_selectedIndex >= 0 && m_selectedIndex < (int)m_sprites.size()) {
        Vector2f pos = m_sprites[m_selectedIndex].getPosition();
        RectangleShape outline(Vector2f((float)m_iconSize, (float)m_iconSize));
        outline.setPosition(pos);
        outline.setFillColor(Color::Transparent);
        outline.setOutlineColor(Color::Green);
        outline.setOutlineThickness(2.f);
        window.draw(outline);
    }

    window.setView(oldView);
}

EnemyType EnemySelector::getSelectedEnemy() const {
    return m_selectedEnemy;
}
