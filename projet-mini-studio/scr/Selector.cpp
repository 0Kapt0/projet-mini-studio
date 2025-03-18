#include "../include/Selector.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

Selector::Selector() {
    if (!backgroundTexture.loadFromFile("assets/selector/background.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    if (!level1Button.loadFromFile("assets/selector/level1.png")) {
        cerr << "Erreur lors du chargement de playtexture." << endl;
    }
    level1Sprite.setTexture(level1Button);
    if (!level2Button.loadFromFile("assets/selector/level2.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    level2Sprite.setTexture(level2Button);
    if (!level3Button.loadFromFile("assets/selector/level3.png")) {
        cerr << "Erreur lors du chargement de settingstexture." << endl;
    }
    level3Sprite.setTexture(level3Button);
}

Selector::~Selector() {
    cout << "Le menu selector est détruit\n";
}

void Selector::draw(RenderWindow& window) {
    //background
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(2.4f, 2.4f);
    window.draw(backgroundSprite);
    //play
    level1Sprite.setScale(0.4f, 0.4f);
    level1Sprite.setPosition(460.0f, 400.0f);
    window.draw(level1Sprite);
    //edit
    level2Sprite.setScale(0.4f, 0.4f);
    level2Sprite.setPosition(960.0f, 400.0f);
    window.draw(level2Sprite);
    //settings
    level3Sprite.setScale(0.4f, 0.4f);
    level3Sprite.setPosition(960.0f, 700.0f);
    window.draw(level3Sprite);
}