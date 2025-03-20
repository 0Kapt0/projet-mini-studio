#ifndef GAME_H
#define GAME_H

#include "Background.hpp"
#include "Foreground.hpp"

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


class Game {
private:
    Background background;
    Foreground foreground;
public:
    void run();
    Game();
    ~Game();
    float deltaTime;
};

#endif