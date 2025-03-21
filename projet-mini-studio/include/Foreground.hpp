#ifndef FOREGROUND_HPP
#define FOREGROUND_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Foreground {
private:
    Texture textures[2];
    Sprite sprites[2];
    float speeds[2] = { 0.f};
    float spriteWidth = 32715.0f;
    float spriteHeight = 4320.0f;
    float offsetX[2] = { 0};

public:
    Foreground();
    bool loadTextures(const string& layer1, const string& layer2);
    void update(float playerX);
    void draw(RenderWindow& window);
};

#endif

