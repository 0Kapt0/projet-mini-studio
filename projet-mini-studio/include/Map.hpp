﻿#ifndef MAP_HPP
#define MAP_HPP

#include "EnemySelector.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

using namespace sf;
using namespace std;

enum class SlopeType {
    None,
    SlopeUp,
    SlopeDown
};

struct TileChange {
    int x;
    int y;
    int oldTile;
    int newTile;
};

struct EnemySpawn {
    string type;
    float x;
    float y;
};

class Map {
public:
    Map(const string& tilesetPath, const string& mapPath);
    ~Map();

    void loadMap(const string& filename);
    void saveMap(const string& filename);
    void handleClick(RenderWindow& window, int x, int y, int tileIndex);
    void handleEnemyPlacement(RenderWindow& window, int x, int y, EnemyType enemyType);
    void handleEnemyRemoval(RenderWindow& window, int x, int y);
    void handleEvent(Event event, RenderWindow& window);
    void draw(RenderWindow& window);
    void drawGrid(RenderWindow& window);
    void drawEnemySpawns(RenderWindow& window);
    void drawCam(RenderWindow& window);
    void generateTiles();
    void setTileset(const std::string& newTilesetPath);

    void undo();
	void redo();
    vector<vector<TileChange>> undoStack;
    vector<vector<TileChange>> redoStack;
    vector<TileChange> currentDragChanges;
    void pushAction(const vector<TileChange>& action);

    bool isColliding(int x, int y) const;

    bool isSlopeTile(int tileX, int tileY) const;
    SlopeType getSlopeTypeAt(int tileX, int tileY) const;
    float getSlopeSurfaceY(int tileX, int tileY, float worldX) const;

    const vector<vector<int>>& getMap() const { return map; }

    vector<EnemySpawn> enemySpawns;
    const View& getCameraView() const { return cameraView; }

    static const int MAP_WIDTH = 300;
    static const int MAP_HEIGHT = 33;
    static const int TILE_SIZE = 64;
private:
    Texture tilesetTexture;
    vector<Sprite> tiles;
    set<int> collisionTiles;
    vector<Vector2i> blockedTiles;

    bool isPanning = false;
    Vector2i lastMousePos;
    float currentZoom = 1.0f;

    View cameraView;
    Vector2f cameraPos;
    float cameraSpeed;
    bool isEditorMode;

    vector<vector<int>> map;

    unordered_map<int, SlopeType> slopeInfo;
};

#endif
