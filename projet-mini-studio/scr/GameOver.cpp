#include "../include/Gameover.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

Gameover::Gameover() {
    if (!backgroundTexture.loadFromFile("assets/gameover/gameover.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    if (!menuTexture.loadFromFile("assets/gameover/menu.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    menuSprite.setTexture(menuTexture);
    if (!overTexture.loadFromFile("assets/gameover/over.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    overSprite.setTexture(overTexture);
}

Gameover::~Gameover() {
    cout << "Le Gameover est détruit\n";
}

void Gameover::draw(RenderWindow& window) {
    //background
    backgroundSprite.setScale(1.0f, 1.0f);
    window.draw(backgroundSprite);
    //
    menuSprite.setScale(1.0f, 1.0f);
    menuSprite.setPosition(500.0f, 800.0f);
    window.draw(menuSprite);
    //
    overSprite.setScale(1.0f, 1.0f);
    overSprite.setPosition(500.0f, 200.0f);
    window.draw(overSprite);
}