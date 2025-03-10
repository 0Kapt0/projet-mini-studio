#include "../include/Map.hpp"
#include <fstream>
#include <iostream>

// Constructeur
Map::Map(const string& tilesetPath, const string& mapPath) {
    if (!tilesetTexture.loadFromFile(tilesetPath)) {
        cerr << "Erreur lors du chargement du tileset." << endl;
    }

    collisionTiles = { 84 };

    loadMap(mapPath);
    generateTiles();
}

Map::~Map() {}

void Map::generateTiles() {
    int tilesetWidth = tilesetTexture.getSize().x / TILE_SIZE;

    tiles.clear();
    blockedTiles.clear();

    for (size_t y = 0; y < map.size(); ++y) {
        for (size_t x = 0; x < map[y].size(); ++x) {
            int tileIndex = map[y][x];

            if (collisionTiles.count(tileIndex)) {
                blockedTiles.push_back(Vector2i(x, y));
            }

            int tileX = (tileIndex % tilesetWidth) * TILE_SIZE;
            int tileY = (tileIndex / tilesetWidth) * TILE_SIZE;

            Sprite sprite;
            sprite.setTexture(tilesetTexture);
            sprite.setTextureRect(IntRect(tileX, tileY, TILE_SIZE, TILE_SIZE));
            sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);

            tiles.push_back(sprite);
        }
    }
}

void Map::loadMap(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        map.clear();

        for (int i = 0; i < MAP_HEIGHT; ++i) {
            vector<int> row;
            for (int j = 0; j < MAP_WIDTH; ++j) {
                int tile;
                file >> tile;
                row.push_back(tile);
            }
            map.push_back(row);
        }

        generateTiles();
    }
    else {
        cerr << "Impossible de charger la carte depuis le fichier." << endl;
    }
}

void Map::saveMap(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& row : map) {
            for (int tile : row) {
                file << tile << " ";
            }
            file << endl;
        }
    }
    else {
        cerr << "Impossible de sauvegarder la carte." << endl;
    }
}

void Map::handleClick(int x, int y, int tileIndex) {
    int mapX = x / TILE_SIZE;
    int mapY = y / TILE_SIZE;

    if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
        map[mapY][mapX] = tileIndex;
        generateTiles();
    }
}

bool Map::isColliding(int x, int y) const {
    Vector2i tilePos(x / TILE_SIZE, y / TILE_SIZE);
    return find(blockedTiles.begin(), blockedTiles.end(), tilePos) != blockedTiles.end();
}

void Map::draw(RenderWindow& window) {
    for (const auto& tile : tiles) {
        window.draw(tile);
    }
}
