#include "../include/Settings.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

Settings::Settings() {
    if (!backgroundTextureSettings.loadFromFile("assets/settingstexture/background.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    if (!setsoundsTexture.loadFromFile("assets/menutexture/playbutton.png")) {
        cerr << "Erreur lors du chargement de playtexture." << endl;
    }
    setsoundsSprite.setTexture(setsoundsTexture);
    if (!setmusicsTexture.loadFromFile("assets/menutexture/editmode.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    setmusicsSprite.setTexture(setmusicsTexture);
    if (!setframesTexture.loadFromFile("assets/menutexture/settings.png")) {
        cerr << "Erreur lors du chargement de settingstexture." << endl;
    }
    setframesSprite.setTexture(setframesTexture);
    if (!returnTexture.loadFromFile("assets/menutexture/menu.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    returnSprite.setTexture(returnTexture);
}

Settings::~Settings() {
    cout << "Le menu settings est détruit\n";
}

void Settings::draw(RenderWindow& window) {
    //background
    Sprite backgroundSprite(backgroundTextureSettings);
    backgroundSprite.setScale(0.5f, 0.5f);
    backgroundSprite.setPosition(0.0f, 0.0f);
    window.draw(backgroundSprite);
}