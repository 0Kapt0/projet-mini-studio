#include "../include/Map.hpp"
#include <fstream>
#include <iostream>

// Constructeur
Map::Map(const string& tilesetPath, const string& mapPath)
    : cameraPos(0, 0), cameraSpeed(50.0f), isEditorMode(false) {

    if (!tilesetTexture.loadFromFile(tilesetPath)) {
        cerr << "Erreur lors du chargement du tileset." << endl;
    }

    cameraView.setSize(1920, 1080);
    cameraView.setCenter(cameraPos);

    collisionTiles = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };

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
        blockedTiles.clear();

        for (int i = 0; i < MAP_HEIGHT; ++i) {
            vector<int> row;
            for (int j = 0; j < MAP_WIDTH; ++j) {
                int tile;
                file >> tile;
                row.push_back(tile);

                if (collisionTiles.count(tile)) {
                    blockedTiles.push_back(Vector2i(j, i));
                }
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

void Map::handleClick(RenderWindow& window, int x, int y, int tileIndex) {
    Vector2f worldPos = window.mapPixelToCoords(Vector2i(x, y));
    int mapX = worldPos.x / TILE_SIZE;
    int mapY = worldPos.y / TILE_SIZE;

    if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
        map[mapY][mapX] = tileIndex;
        generateTiles();
    }
}

void Map::handleEvent(Event event) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Z || event.key.code == Keyboard::Up) {
            cameraPos.y -= cameraSpeed;
        }
        if (event.key.code == Keyboard::S || event.key.code == Keyboard::Down) {
            cameraPos.y += cameraSpeed;
        }
        if (event.key.code == Keyboard::Q || event.key.code == Keyboard::Left) {
            cameraPos.x -= cameraSpeed;
        }
        if (event.key.code == Keyboard::D || event.key.code == Keyboard::Right) {
            cameraPos.x += cameraSpeed;
        }
    }
}


bool Map::isColliding(int x, int y) const {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    //Vérification des limites de la carte
    if (tileX < 0 || tileY < 0 || tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT) {
        return true;
    }

    //Vérification si la tuile est bloquante
    bool isBlocked = std::find(blockedTiles.begin(), blockedTiles.end(), Vector2i(tileX, tileY)) != blockedTiles.end();
    return isBlocked;
}


void Map::draw(RenderWindow& window) {
    for (const auto& tile : tiles) {
        window.draw(tile);
    }
}

void Map::drawCam(RenderWindow& window) {
    cameraView.setCenter(cameraPos);
    window.setView(cameraView);
}

