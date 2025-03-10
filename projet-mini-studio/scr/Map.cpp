#include "../include/Map.hpp"
#include <fstream>
#include <iostream>

//Constructeur
Map::Map(const string& tilesetPath, const string& mapPath) {
    if (!tilesetTexture.loadFromFile(tilesetPath)) {
        cerr << "Erreur lors du chargement du tileset." << endl;
    }

    //Découpe du tileset en tuiles
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            Sprite sprite;
            sprite.setTexture(tilesetTexture);
            sprite.setTextureRect(IntRect(i * TILE_SIZE, j * TILE_SIZE, TILE_SIZE, TILE_SIZE));
            tiles.push_back(sprite);
        }
    }

    loadMap(mapPath);
}

Map::~Map() {}

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
    }
    else {
        cerr << "Impossible de charger la carte depuis le fichier." << endl;
    }
}

//Sauvegarde de la carte dans un fichier texte
void Map::saveMap(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < MAP_HEIGHT; ++i) {
            for (int j = 0; j < MAP_WIDTH; ++j) {
                file << map[i][j] << " ";
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
    }
}


void Map::draw(RenderWindow& window) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            Sprite sprite = tiles[map[i][j]];  // Récupère la tuile correspondante
            sprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            window.draw(sprite);
        }
    }
}
