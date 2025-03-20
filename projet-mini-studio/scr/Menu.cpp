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
    playSprite.setTexture(playButton);
    if (!editmodeButton.loadFromFile("assets/menutexture/editmode.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    editSprite.setTexture(editmodeButton);
    if (!settingsButton.loadFromFile("assets/menutexture/settings.png")) {
        cerr << "Erreur lors du chargement de settingstexture." << endl;
    }
    settingSprite.setTexture(settingsButton);
    if (!menuButton.loadFromFile("assets/menutexture/menu.png")) {
        cerr << "Erreur lors du chargement de editmodetexture." << endl;
    }
    menuSprite.setTexture(menuButton);
}

Menu::~Menu() {
    cout << "Le menu est détruit\n";
}

void Menu::draw(RenderWindow& window) {
    //background
    Sprite backgroundSprite(backgroundTexture);
    window.draw(backgroundSprite);
    //play
    playSprite.setPosition(550.0f, 760.0f);
    window.draw(playSprite);
    //edit
    editSprite.setPosition(840.0f, 840.0f);
    window.draw(editSprite);

    //settings
    settingSprite.setPosition(850.0f, 920.0f);
    window.draw(settingSprite);
    //menu
    menuSprite.setPosition(850.0f, 1000.0f);
    window.draw(menuSprite);
}