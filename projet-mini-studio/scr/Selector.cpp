#include "../include/Selector.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

Selector::Selector() {
    levelunlocked = 1;
    if (!backgroundTexture.loadFromFile("assets/selector/background.png")) {
    }
    if (!level1Button.loadFromFile("assets/selector/level1.png")) {
    }
    level1Sprite.setTexture(level1Button);
    if (!returnTexture.loadFromFile("assets/selector/return.png")) {
    }
    returnSprite.setTexture(returnTexture);
    if (!level2Button.loadFromFile("assets/selector/level2.png")) {
    }
    level2Sprite.setTexture(level2Button);
    if (!level2lockButton.loadFromFile("assets/selector/level2lock.png")) {
    }
    level2lockSprite.setTexture(level2lockButton);
    if (!level3lockButton.loadFromFile("assets/selector/level3lock.png")) {
    }
    level3lockSprite.setTexture(level3lockButton);
    if (!level3Button.loadFromFile("assets/selector/level3.png")) {
    }
    level3Sprite.setTexture(level3Button);
    if (!ATexture.loadFromFile("assets/icones/A.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    if (!BTexture.loadFromFile("assets/icones/B.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    if (!YTexture.loadFromFile("assets/icones/Y.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    if (!XTexture.loadFromFile("assets/icones/X.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    ASprite.setTexture(ATexture);
    BSprite.setTexture(BTexture);
    YSprite.setTexture(YTexture);
    XSprite.setTexture(XTexture);
    ASprite.setScale(0.65, 0.65);
    BSprite.setScale(0.35, 0.35);
    YSprite.setScale(0.65, 0.65);
    XSprite.setScale(0.65, 0.65);
}

Selector::~Selector() {
    cout << "Le menu selector est détruit\n";
}

void Selector::draw(RenderWindow& window) {
    //background
    Sprite backgroundSprite(backgroundTexture);
    window.draw(backgroundSprite);
    //level1
    level1Sprite.setPosition(550.0f, 300.0f);
    window.draw(level1Sprite);
    //level2
    level2Sprite.setPosition(550.0f, 500.0f);
    window.draw(level2Sprite);
    //level2lock
    if (levelunlocked < 2 ) {
        level2lockSprite.setPosition(550.0f, 500.0f);
        window.draw(level2lockSprite);
    }
    //level3
    level3Sprite.setPosition(550.0f, 700.0f);
    window.draw(level3Sprite);
    //level3lock
    if (levelunlocked < 3) {
        level3lockSprite.setPosition(550.0f, 700.0f);
        window.draw(level3lockSprite);
    }
    //return
    returnSprite.setPosition(50.0f, 1000.0f);
    window.draw(returnSprite);
    ASprite.setPosition(460.0f, 330.0f);
    window.draw(ASprite);
    BSprite.setPosition(350.0f, 1000.0f);
    window.draw(BSprite);
    YSprite.setPosition(460.0f, 530.0f);
    window.draw(YSprite);
    XSprite.setPosition(460.0f, 735.0f);
    window.draw(XSprite);
}