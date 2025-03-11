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

    int getTilesetWidth() const {
        return tilesetTexture.getSize().x / tileSize;
    }

    int getTilesetHeight() const {
        return tilesetTexture.getSize().y / tileSize;
    }

    int getTileSize() const {
        return tileSize;
    }


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
