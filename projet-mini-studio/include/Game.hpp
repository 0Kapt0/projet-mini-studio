#ifndef GAME_H
#define GAME_H

#include "Background.hpp"
#include "Foreground.hpp"
#include "Map.hpp"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum class PlacementMode {
    Tiles,
    Enemies
};

class Game {
private:
    Background background;
    Foreground foreground;
	bool showGrid = true;
    bool enemiesGenerated = false;
    Font font;
	View oldView;
    EnemySpawn spawn;


    PlacementMode currentPlacementMode = PlacementMode::Tiles;
public:
    void run();
    Game();
    ~Game();
    float deltaTime;
};

#endif