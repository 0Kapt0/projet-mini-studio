#include "../include/Map.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>

Map::Map(const string& tilesetPath, const string& mapPath)
    : cameraPos(0, 0), cameraSpeed(50.0f), isEditorMode(false)
{
    if (!tilesetTexture.loadFromFile(tilesetPath)) {
        cerr << "Erreur lors du chargement du tileset." << endl;
    }

    cameraView.setSize(1920, 1080);
    cameraView.setCenter(cameraPos);

    collisionTiles = { 1,2,3,4,5,6,7,8,9,10,11,12 };

    slopeInfo[50] = SlopeType::SlopeUp;
    slopeInfo[65] = SlopeType::SlopeUp;

    slopeInfo[51] = SlopeType::SlopeDown;
    slopeInfo[68] = SlopeType::SlopeDown;

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
            sprite.setPosition(float(x * TILE_SIZE), float(y * TILE_SIZE));
            tiles.push_back(sprite);
        }
    }
}

void Map::loadMap(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Impossible de charger la carte depuis le fichier." << endl;
        return;
    }

    map.clear();
    blockedTiles.clear();
    enemySpawns.clear();

    string line;
    bool readingTiles = false;
    bool readingEnemies = false;
    int tilesRead = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line == "#TILES") {
            readingTiles = true;
            readingEnemies = false;
            tilesRead = 0;
            continue;
        }
        else if (line == "#ENEMIES") {
            readingTiles = false;
            readingEnemies = true;
            continue;
        }

        if (readingTiles && tilesRead < MAP_HEIGHT) {
            stringstream ss(line);
            vector<int> row;
            row.reserve(MAP_WIDTH);

            for (int j = 0; j < MAP_WIDTH; ++j) {
                int tileVal;
                ss >> tileVal;
                row.push_back(tileVal);
            }
            map.push_back(row);
            tilesRead++;
        }
        else if (readingEnemies) {
            stringstream ss(line);
            string type;
            float x, y;
            ss >> type >> x >> y;

            EnemySpawn spawn;
            spawn.type = type;
            spawn.x = x;
            spawn.y = y;
            enemySpawns.push_back(spawn);
        }
    }

    generateTiles();

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            int tileVal = map[i][j];
            if (collisionTiles.count(tileVal)) {
                blockedTiles.push_back(Vector2i(j, i));
            }
        }
    }
}


void Map::saveMap(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Impossible de sauvegarder la carte." << endl;
        return;
    }

    file << "#TILES\n";
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            file << map[i][j] << " ";
        }
        file << "\n";
    }

    file << "#ENEMIES\n";
    for (auto& spawn : enemySpawns) {
        file << spawn.type << " "
            << spawn.x << " "
            << spawn.y << "\n";
    }
}

void Map::handleClick(RenderWindow& window, int x, int y, int tileIndex)
{
    Vector2f worldPos = window.mapPixelToCoords(Vector2i(x, y));
    int mapX = int(worldPos.x / TILE_SIZE);
    int mapY = int(worldPos.y / TILE_SIZE);

    if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT) {
        int oldValue = map[mapY][mapX];
        int newValue = tileIndex;

        if (oldValue != newValue) {
            map[mapY][mapX] = newValue;
            generateTiles();

            TileChange change;
            change.x = mapX;
            change.y = mapY;
            change.oldTile = oldValue;
            change.newTile = newValue;
            currentDragChanges.push_back(change);
        }
    }
}

void Map::pushAction(const vector<TileChange>& action)
{
    redoStack.clear();
    undoStack.push_back(action);
}

//Annuler la derniere action
void Map::undo() {
    if (undoStack.empty()) {
        cout << "Rien à annuler.\n";
        return;
    }

    auto lastAction = undoStack.back();
    undoStack.pop_back();

    for (auto& change : lastAction) {
        map[change.y][change.x] = change.oldTile;
    }
    generateTiles();

    redoStack.push_back(lastAction);
}

//Retablir la derniere action annulee
void Map::redo() {
    if (redoStack.empty()) {
        cout << "Rien à retablir.\n";
        return;
    }

    auto lastUndoneAction = redoStack.back();
    redoStack.pop_back();

    for (auto& change : lastUndoneAction) {
        map[change.y][change.x] = change.newTile;
    }
    generateTiles();

    undoStack.push_back(lastUndoneAction);

    cout << "Redo effectué. undoStack size = " << undoStack.size()
        << ", redoStack size = " << redoStack.size() << endl;
}

void Map::handleEvent(Event event, RenderWindow& window)
{
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Middle) {
            isPanning = true;
            lastMousePos = Mouse::getPosition(window);
        }
    }
    else if (event.type == Event::MouseButtonReleased) {
        if (event.mouseButton.button == Mouse::Middle) {
            isPanning = false;
        }
    }
    else if (event.type == Event::MouseMoved) {
        if (isPanning) {
            Vector2i newPos = Mouse::getPosition(window);

            Vector2f oldWorldPos = window.mapPixelToCoords(lastMousePos, cameraView);
            Vector2f newWorldPos = window.mapPixelToCoords(newPos, cameraView);

            Vector2f delta = oldWorldPos - newWorldPos;

            cameraView.move(delta);
            lastMousePos = newPos;
        }
    }

    else if (event.type == Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
            float delta = event.mouseWheelScroll.delta;

            float factor = (delta > 0) ? 0.9f : 1.1f;

            float newZoom = currentZoom * factor;
            if (newZoom < 0.1f) newZoom = 0.1f;
            if (newZoom > 5.0f)  newZoom = 5.0f;

            float actualFactor = newZoom / currentZoom;
            currentZoom = newZoom;

            cameraView.zoom(actualFactor);
        }
    }
}

bool Map::isColliding(int x, int y) const {
    int tileX = x / TILE_SIZE;
    int tileY = y / TILE_SIZE;

    if (tileX < 0 || tileY < 0 || tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT) {
        return true;
    }

    SlopeType stype = getSlopeTypeAt(tileX, tileY);
    if (stype != SlopeType::None) {
        return false;
    }

    bool isBlocked = (find(blockedTiles.begin(), blockedTiles.end(),
        Vector2i(tileX, tileY)) != blockedTiles.end());
    return isBlocked;
}

void Map::draw(RenderWindow& window) {
    for (const auto& tile : tiles) {
        window.draw(tile);
    }
}

void Map::drawCam(RenderWindow& window) {
    window.setView(cameraView);
}

bool Map::isSlopeTile(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT)
        return false;

    int tileID = map[tileY][tileX];
    auto it = slopeInfo.find(tileID);
    return (it != slopeInfo.end() && it->second != SlopeType::None);
}

SlopeType Map::getSlopeTypeAt(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= MAP_WIDTH || tileY >= MAP_HEIGHT)
        return SlopeType::None;

    int tileID = map[tileY][tileX];
    auto it = slopeInfo.find(tileID);
    if (it != slopeInfo.end()) {
        return it->second;
    }
    return SlopeType::None;
}

float Map::getSlopeSurfaceY(int tileX, int tileY, float worldX) const {
    SlopeType stype = getSlopeTypeAt(tileX, tileY);
    if (stype == SlopeType::None) {
        return (tileY + 1) * TILE_SIZE;
    }

    float tileLeft = tileX * TILE_SIZE;
    float tileTop = tileY * TILE_SIZE - 2;
    float localX = worldX - tileLeft;

    switch (stype)
    {
    case SlopeType::SlopeUp:
        return tileTop + (TILE_SIZE - localX);
    case SlopeType::SlopeDown:
        return tileTop + localX;
    default:
        break;
    }
    return tileTop + TILE_SIZE;
}

void Map::drawGrid(RenderWindow& window) {
    int maxX = MAP_WIDTH * TILE_SIZE;
    int maxY = MAP_HEIGHT * TILE_SIZE;

    Vector2f topLeft = window.getView().getCenter() - window.getView().getSize() / 2.f;
    Vector2f bottomRight = window.getView().getCenter() + window.getView().getSize() / 2.f;

    int startX = max(0, int(topLeft.x / TILE_SIZE) * TILE_SIZE);
    int endX = min(maxX, int(bottomRight.x / TILE_SIZE + 1) * TILE_SIZE);
    int startY = max(0, int(topLeft.y / TILE_SIZE) * TILE_SIZE);
    int endY = min(maxY, int(bottomRight.y / TILE_SIZE + 1) * TILE_SIZE);

    for (int x = startX; x <= endX; x += TILE_SIZE) {
        Vertex line[] =
        {
            Vertex(Vector2f(float(x), float(startY)), Color::White),
            Vertex(Vector2f(float(x), float(endY)),   Color::White)
        };
        window.draw(line, 2, Lines);
    }

    for (int y = startY; y <= endY; y += TILE_SIZE) {
        Vertex line[] =
        {
            Vertex(Vector2f(float(startX), float(y)), Color::White),
            Vertex(Vector2f(float(endX),   float(y)), Color::White)
        };
        window.draw(line, 2, Lines);
    }

    if (topLeft.x < 0 && bottomRight.x > 0) {
        Vertex yAxis[] =
        {
            Vertex(Vector2f(0.f, float(startY)), Color::Red),
            Vertex(Vector2f(0.f, float(endY)),   Color::Red)
        };
        window.draw(yAxis, 2, Lines);
    }

    if (topLeft.y < 0 && bottomRight.y > 0) {
        Vertex xAxis[] =
        {
            Vertex(Vector2f(float(startX), 0.f), Color::Red),
            Vertex(Vector2f(float(endX),   0.f), Color::Red)
        };
        window.draw(xAxis, 2, Lines);
    }
}
