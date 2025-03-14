﻿#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <set>

using namespace sf;
using namespace std;

class Map {
public:
    //Constructeur et destructeur
    Map(const string& tilesetPath, const string& mapPath);
    ~Map();

    void loadMap(const string& filename);
    void saveMap(const string& filename);
    void handleClick(RenderWindow& window, int x, int y, int tileIndex);
	void handleEvent(Event event);
    void draw(RenderWindow& window);
    void drawCam(RenderWindow& window);
    void generateTiles();

    bool isColliding(int x, int y) const;
    const vector<vector<int>>& getMap() const { return map; }

    //Dimensions de la carte
    static const int MAP_WIDTH = 60;
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
};

#endif
