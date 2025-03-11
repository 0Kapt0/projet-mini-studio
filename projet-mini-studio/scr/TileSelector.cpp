#include "../include/TileSelector.hpp"
#include <iostream>

TileSelector::TileSelector(const string& tilesetPath, int tileSize)
    : tileSize(tileSize), selectedTileIndex(-1) {

    if (!tilesetTexture.loadFromFile(tilesetPath)) {
        cerr << "Erreur lors du chargement du tileset." << endl;
    }

    int tilesetWidth = tilesetTexture.getSize().x / tileSize;
    int tilesetHeight = tilesetTexture.getSize().y / tileSize;

    for (int y = 0; y < tilesetHeight; ++y) {
        for (int x = 0; x < tilesetWidth; ++x) {
            Sprite sprite;
            sprite.setTexture(tilesetTexture);
            sprite.setTextureRect(IntRect(x * tileSize, y * tileSize, tileSize, tileSize));
            sprite.setPosition(x * tileSize, y * tileSize);
            tiles.push_back(sprite);
        }
    }
}

void TileSelector::handleEvent(Event event, RenderWindow& window) {
    if (event.type == Event::MouseButtonPressed) {
        Vector2i mousePos = Mouse::getPosition(window);

        //Vérifier si on clique dans la zone de la palette
        int tilesetWidth = tilesetTexture.getSize().x / tileSize;
        int tilesetHeight = tilesetTexture.getSize().y / tileSize;

        if (mousePos.x < tilesetWidth * tileSize && mousePos.y < tilesetHeight * tileSize) {
            int tileX = mousePos.x / tileSize;
            int tileY = mousePos.y / tileSize;
            int index = tileY * tilesetWidth + tileX;

            if (index >= 0 && index < tiles.size()) {
                selectedTileIndex = index;
                cout << "Tuile sélectionnée : " << selectedTileIndex << endl;
            }
        }
    }
}

void TileSelector::toggleCollision() {
    if (selectedTileIndex != -1) {
        if (collisionTiles.count(selectedTileIndex))
            collisionTiles.erase(selectedTileIndex);
        else
            collisionTiles.insert(selectedTileIndex);
    }
}
void TileSelector::draw(RenderWindow& window) {
    Vector2f viewPos = window.getView().getCenter() - (window.getView().getSize() / 2.f);
    int tilesetWidth = tilesetTexture.getSize().x / tileSize;
    for (size_t i = 0; i < tiles.size(); ++i) {
        tiles[i].setPosition(viewPos.x + (i % tilesetWidth) * tileSize,
            viewPos.y + (i / tilesetWidth) * tileSize);
        window.draw(tiles[i]);
    }

    if (selectedTileIndex != -1) {
        bool hasCollision = collisionTiles.count(selectedTileIndex);

        RectangleShape outline(Vector2f(tileSize, tileSize));
        outline.setPosition(viewPos.x + (selectedTileIndex % tilesetWidth) * tileSize,
            viewPos.y + (selectedTileIndex / tilesetWidth) * tileSize);
        outline.setFillColor(Color::Transparent);
        outline.setOutlineColor(hasCollision ? Color::Red : Color::Green);
        outline.setOutlineThickness(2);
        window.draw(outline);
    }
}


int TileSelector::getSelectedTile() const {
    return selectedTileIndex;
}
