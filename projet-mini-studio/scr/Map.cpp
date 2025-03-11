#include "../include/Map.hpp"
#include <fstream>
#include <unordered_set>
#include <iostream>
#include <sstream>qqq

unordered_set<int> ignoredTiles = { 74 };

// Constructeur
Map::Map(const string& tilesetPath, const string& mapPath)
    : cameraPos(0, 0), cameraSpeed(50.0f), isEditorMode(false) {

    if (!tilesetTexture.loadFromFile(tilesetPath)) {
        cerr << "Erreur lors du chargement du tileset." << endl;
    }

    cameraView.setSize(1920, 1080);
    cameraView.setCenter(cameraPos);

    collisionTiles = { 1 };

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

            /*sprite.setScale(0.5f, 0.5f);*/

            tiles.push_back(sprite);
        }
    }
}

void Map::loadMap(const string& mapPath) {
    std::ifstream file(mapPath);
    if (!file) {
        std::cerr << "Erreur : Impossible de charger la carte " << mapPath << std::endl;
        return;
    }

    map.clear();
    string line;
    while (getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        int tile;
        while (ss >> tile) {
            row.push_back(tile);
        }
        map.push_back(row);
    }

    // DEBUG : Afficher la carte chargée
    std::cout << "Carte chargée : " << std::endl;
    for (size_t y = 0; y < map.size(); ++y) {
        for (size_t x = 0; x < map[y].size(); ++x) {
            std::cout << map[y][x] << " ";
        }
        std::cout << std::endl;
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

    // Vérifier si la position est hors limites
    if (tileY < 0 || tileY >= map.size() || tileX < 0 || tileX >= map[0].size()) {
        std::cout << "Collision détectée : Hors limites (" << tileX << ", " << tileY << ")" << std::endl;
        return true; // Collision si hors des limites de la carte
    }

    int tileID = map[tileY][tileX];

    // DEBUG : Afficher la tuile détectée et ses coordonnées
    std::cout << "Tuile détectée: " << tileID << " (" << tileX << ", " << tileY << ")" << std::endl;

    // Si la tuile est dans `ignoredTiles`, elle n'a PAS de collision
    if (ignoredTiles.count(tileID)) {
        std::cout << "Tuile ignorée: " << tileID << std::endl;
        return false;
    }

    // Toutes les autres tuiles provoquent une collision
    std::cout << "Test collision : Tuile=" << tileID
        << " (" << tileX << "," << tileY << ") "
        << (ignoredTiles.count(tileID) ? "IGNOREE" : "COLLISION")
        << std::endl;

    return true;
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

