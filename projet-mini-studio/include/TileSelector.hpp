#ifndef TILESELECTOR_HPP
#define TILESELECTOR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <set>

using namespace sf;
using namespace std;

class TileSelector {
public:
    TileSelector(const string& tilesetPath, int tileSize);

    void handleEvent(Event event, RenderWindow& window);
    void draw(RenderWindow& window);
    int getSelectedTile() const;
    void toggleCollision();

private:
    Texture tilesetTexture;
    vector<Sprite> tiles;
    set<int> collisionTiles;
    int tileSize;
    int selectedTileIndex;
};

#endif
