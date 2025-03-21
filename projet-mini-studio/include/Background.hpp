#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <SFML/Graphics.hpp>
#include <array>
#include <string>

using namespace sf;
using namespace std;

class Background {
private:
    Texture textures[6];
    Sprite sprites[6];
    float speeds[6] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
    float spriteWidth = 32715.0f;
    float spriteHeight = 2160.0f;

public:
    Background();
    bool loadTextures(const std::string& layer1,
        const string& layer2,
        const string& layer3,
        const string& layer4,
        const string& layer5,
        const string& layer6);

    void setSpeeds(const array<float, 6>& newSpeeds);

    void update(float cameraX , bool followCamera = true);

    void draw(RenderWindow& window);
};

#endif
