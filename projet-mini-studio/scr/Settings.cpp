#include "../include/Settings.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

Settings::Settings() {
    if (!backgroundTextureSettings.loadFromFile("assets/settingstexture/background.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
}

Settings::~Settings() {
    cout << "Le menu settings est détruit\n";
}

void Settings::draw(RenderWindow& window) {
    //background
    Sprite backgroundSprite(backgroundTextureSettings);
    backgroundSprite.setScale(3.0f, 3.2f);
    backgroundSprite.setPosition(0.0f, 0.0f);
    window.draw(backgroundSprite);
}