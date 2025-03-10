#include "../include/Menu.hpp"
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

Menu::Menu() {
    if (!backgroundTexture.loadFromFile("assets/menutexture/menubackground.png")) {
        cerr << "Erreur lors du chargement de la texture de fond." << endl;
    }
    if (!playButton.loadFromFile("assets/menutexture/playbutton.png")) {
        cerr << "Erreur lors du chargement de playtexture." << endl;
    }
    if (!editmodeButton.loadFromFile("assets/menutexture/editmode.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    editSprite.setTexture(editmodeButton);
    if (!settingsButton.loadFromFile("assets/menutexture/settings.png")) {
        cerr << "Erreur lors du chargement de settingstexture." << endl;
    }
    if (!menuButton.loadFromFile("assets/menutexture/menu.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
}

Menu::~Menu() {
    cout << "Le menu est détruit\n";
}

void Menu::draw(RenderWindow& window) {
    //background
    Sprite backgroundSprite(backgroundTexture);
    window.draw(backgroundSprite);
    //play
    Sprite playSprite(playButton);
    playSprite.setScale(0.4f,0.4f);
    playSprite.setPosition(460.0f, 400.0f);
    window.draw(playSprite);
    //edit
    editSprite.setScale(0.4f, 0.4f);
    editSprite.setPosition(960.0f, 400.0f);
    window.draw(editSprite);

    //settings
    Sprite settingsSprite(settingsButton);
    settingsSprite.setScale(0.4f, 0.4f);
    settingsSprite.setPosition(960.0f, 700.0f);
    window.draw(settingsSprite);
    //menu
    Sprite menuSprite(menuButton);
    menuSprite.setScale(0.4f, 0.4f);
    menuSprite.setPosition(460.0f, 700.0f);
    window.draw(menuSprite);
}