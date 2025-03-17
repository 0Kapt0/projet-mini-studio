#include "../include/TileSelector.hpp"
#include <iostream>

TileSelector::TileSelector(const std::string& tilesetPath, int tileSize)
    : tileSize(tileSize), selectedTileIndex(-1)
{
    if (!tilesetTexture.loadFromFile(tilesetPath)) {
        cerr << "Erreur lors du chargement du tileset." << std::endl;
    }

    int tilesetWidth = tilesetTexture.getSize().x / tileSize;
    int tilesetHeight = tilesetTexture.getSize().y / tileSize;

    for (int y = 0; y < tilesetHeight; ++y) {
        for (int x = 0; x < tilesetWidth; ++x) {
            sf::Sprite sprite;
            sprite.setTexture(tilesetTexture);
            sprite.setTextureRect(sf::IntRect(x * tileSize, y * tileSize, tileSize, tileSize));
            sprite.setPosition(float(x * tileSize), float(y * tileSize));
            tiles.push_back(sprite);
        }
    }
}

void TileSelector::handleEvent(Event event, RenderWindow& window)
{
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);

            int tilesetWidth = tilesetTexture.getSize().x / tileSize;
            int tilesetHeight = tilesetTexture.getSize().y / tileSize;

            int paletteWidth = tilesetWidth * tileSize;
            int paletteHeight = tilesetHeight * tileSize;

            if (mousePos.x >= 0 && mousePos.x < paletteWidth &&
                mousePos.y >= 0 && mousePos.y < paletteHeight)
            {
                int tileX = mousePos.x / tileSize;
                int tileY = mousePos.y / tileSize;
                int index = tileY * tilesetWidth + tileX;

                if (index >= 0 && index < static_cast<int>(tiles.size())) {
                    selectedTileIndex = index;
                    std::cout << "Tuile choisie : " << selectedTileIndex << std::endl;
                }
            }
        }
    }
}

void TileSelector::toggleCollision()
{
    if (selectedTileIndex != -1) {
        if (collisionTiles.count(selectedTileIndex))
            collisionTiles.erase(selectedTileIndex);
        else
            collisionTiles.insert(selectedTileIndex);
    }
}

void TileSelector::draw(sf::RenderWindow& window)
{
    View oldView = window.getView();

    window.setView(window.getDefaultView());

    int tilesetWidth = tilesetTexture.getSize().x / tileSize;
    for (size_t i = 0; i < tiles.size(); ++i)
    {
        float posX = float((i % tilesetWidth) * tileSize);
        float posY = float((i / tilesetWidth) * tileSize);
        tiles[i].setPosition(posX, posY);
        window.draw(tiles[i]);
    }

    if (selectedTileIndex != -1) {
        bool hasCollision = collisionTiles.count(selectedTileIndex);

        float outlineX = float((selectedTileIndex % tilesetWidth) * tileSize);
        float outlineY = float((selectedTileIndex / tilesetWidth) * tileSize);

        RectangleShape outline(Vector2f(tileSize, tileSize));
        outline.setPosition(outlineX, outlineY);
        outline.setFillColor(Color::Transparent);
        outline.setOutlineColor(hasCollision ? Color::Red : Color::Green);
        outline.setOutlineThickness(2);
        window.draw(outline);
    }
    window.setView(oldView);
}

int TileSelector::getSelectedTile() const {
    return selectedTileIndex;
}

const unordered_set<int>& TileSelector::getCollisionTiles() const {
    return collisionTiles;
}
