#ifndef MAP_HPP
#define MAP_HPP

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

class Map {
public:
    Map(const string& tilesetPath, const string& mapPath);
    ~Map();

    void loadMap(const string& filename);
    void saveMap(const string& filename);
    void handleClick(RenderWindow& window, int x, int y, int tileIndex);
    void handleEvent(Event event);
    void draw(RenderWindow& window);
    void drawGrid(RenderWindow& window);
    void drawCam(RenderWindow& window);
    void generateTiles();

    bool isColliding(int x, int y) const;

    bool isSlopeTile(int tileX, int tileY) const;
    SlopeType getSlopeTypeAt(int tileX, int tileY) const;
    float getSlopeSurfaceY(int tileX, int tileY, float worldX) const;

    const vector<vector<int>>& getMap() const { return map; }

    static const int MAP_WIDTH = 300;
    static const int MAP_HEIGHT = 33;
    static const int TILE_SIZE = 64;

private:
    Texture tilesetTexture;
    vector<Sprite> tiles;
    set<int> collisionTiles;
    vector<Vector2i> blockedTiles;

    View cameraView;
    Vector2f cameraPos;
    float cameraSpeed;
    bool isEditorMode;

    vector<vector<int>> map;

    unordered_map<int, SlopeType> slopeInfo;
};

#endif
