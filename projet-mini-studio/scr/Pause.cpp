#include "../include/Pause.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

Pause::Pause() {
    if (!backgroundTexture.loadFromFile("assets/pausetexture/menupause.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    if (!playButtonPause.loadFromFile("assets/pausetexture/playbutton.png")) {
        cerr << "Erreur lors du chargement de playtexture." << endl;
    }
    playSpritePause.setTexture(playButtonPause);
    if (!menuButtonPause.loadFromFile("assets/pausetexture/menu.png")) {
        cerr << "Erreur lors du chargement de settingstexture." << endl;
    }
    menuSpritePause.setTexture(menuButtonPause);
}

Pause::~Pause() {
    cout << "Le menu pause est détruit\n";
}

void Pause::draw(RenderWindow& window) {
    //background
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(2.7f, 2.9f);
    backgroundSprite.setPosition(0.0f, 0.0f);
    window.draw(backgroundSprite);
    //play
    playSpritePause.setScale(0.4f, 0.4f);
    playSpritePause.setPosition(460.0f, 400.0f);
    window.draw(playSpritePause);
    //menu
    menuSpritePause.setScale(0.4f, 0.4f);
    menuSpritePause.setPosition(1060.0f, 400.0f);
    window.draw(menuSpritePause);
}