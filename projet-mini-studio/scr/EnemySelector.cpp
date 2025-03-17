#include "../include/EnemySelector.hpp"
#include <iostream>

EnemySelector::EnemySelector(const std::string& texturePath, int iconSize)
    : m_iconSize(iconSize)
    , m_selectedIndex(-1)
    , m_selectedEnemy(EnemyType::Flying) // par d�faut
{
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Erreur chargement texture ennemis\n";
    }

    // Imaginons que la texture contienne 4 ou 5 ic�nes
    // en 1 ligne (ex: 4 colonnes � 64 pixels = 256 px de large)
    // On remplit le tableau m_enemyIcons
    // On suppose 4 types d�ennemis, chacun un carr� de iconSize
    int textureWidth = m_texture.getSize().x;
    int textureHeight = m_texture.getSize().y;

    int countIcons = textureWidth / iconSize;

    // On pr�pare le mapping index->EnemyType
    // (Il faut que �a corresponde dans le bon ordre.)
    m_enemyTypes = {
        EnemyType::Flying,
        EnemyType::Ranged,
        EnemyType::Basic,
        EnemyType::ChargingBoss
        // Compl�ter si vous avez plus
    };

    for (int i = 0; i < countIcons; i++) {
        sf::Sprite sprite;
        sprite.setTexture(m_texture);
        sprite.setTextureRect(
            sf::IntRect(i * iconSize, 0, iconSize, iconSize));
        // On place la premi�re ic�ne � (0,0), la seconde � (0+64,0), etc.
        sprite.setPosition(float(i * iconSize), 0.f);
        m_enemyIcons.push_back(sprite);
    }
}

void EnemySelector::handleEvent(sf::Event event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Calculer la zone occup�e par la palette
        // ex: largeur = m_enemyIcons.size() * m_iconSize, hauteur = m_iconSize
        int paletteWidth = int(m_enemyIcons.size()) * m_iconSize;
        int paletteHeight = m_iconSize;

        if (mousePos.x >= 0 && mousePos.x < paletteWidth &&
            mousePos.y >= 0 && mousePos.y < paletteHeight)
        {
            // On clique sur l�une des ic�nes
            int indexClicked = mousePos.x / m_iconSize;
            if (indexClicked >= 0 && indexClicked < (int)m_enemyIcons.size()) {
                m_selectedIndex = indexClicked;
                m_selectedEnemy = m_enemyTypes[indexClicked];

                std::cout << "Ennemi s�lectionn�, index=" << indexClicked << std::endl;
            }
        }
    }
}

void EnemySelector::draw(sf::RenderWindow& window)
{
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    // Dessin des ic�nes
    for (size_t i = 0; i < m_enemyIcons.size(); i++) {
        float posX = float(i * m_iconSize);
        float posY = 0.f;
        m_enemyIcons[i].setPosition(posX, posY);
        window.draw(m_enemyIcons[i]);
    }

    if (m_selectedIndex >= 0 && m_selectedIndex < (int)m_enemyIcons.size()) {
        float outlineX = float(m_selectedIndex * m_iconSize);
        float outlineY = 0.f;

        sf::RectangleShape outline(sf::Vector2f((float)m_iconSize, (float)m_iconSize));
        outline.setPosition(outlineX, outlineY);
        outline.setFillColor(sf::Color::Transparent);
        outline.setOutlineColor(sf::Color::Yellow);
        outline.setOutlineThickness(2.f);

        window.draw(outline);
    }

    window.setView(oldView);
}
