#include "../include/Cutscene.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../include/SoundManager.hpp"

using namespace sf;
using namespace std;

Cutscene::Cutscene() {
    for (int i = 4; i < 133; ++i) {
        // Utiliser std::ostringstream pour formater le numéro avec un nombre fixe de 5 chiffres
        ostringstream filename;
        filename << "assets/cutscene/cutscene2/Composition 1_" << setw(5) << setfill('0') << i << ".jpg";

        if (!cutscene2Texture[i].loadFromFile(filename.str())) {
            cerr << "Erreur lors du chargement de l'image " << filename.str() << endl;
        }
    } 
    for (int i = 0; i < 89; ++i) {
        // Utiliser std::ostringstream pour formater le numéro avec un nombre fixe de 5 chiffres
        ostringstream filename;
        filename << "assets/cutscene/cutscene1/Composition 2_" << setw(5) << setfill('0') << i << ".jpg";

        if (!cutscene1Texture[i].loadFromFile(filename.str())) {
            cerr << "Erreur lors du chargement de l'image " << filename.str() << endl;
        }
    }
    for (int i = 0; i < 84; ++i) {
        // Utiliser std::ostringstream pour formater le numéro avec un nombre fixe de 5 chiffres
        ostringstream filename;
        filename << "assets/cutscene/cutscene3/Composition 3_" << setw(5) << setfill('0') << i << ".jpg";

        if (!cutscene3Texture[i].loadFromFile(filename.str())) {
            cerr << "Erreur lors du chargement de l'image " << filename.str() << endl;
        }
    }
    for (int i = 0; i < 84; ++i) {
        // Utiliser std::ostringstream pour formater le numéro avec un nombre fixe de 5 chiffres
        ostringstream filename;
        filename << "assets/cutscene/cutscene4/Composition 4_" << setw(5) << setfill('0') << i << ".jpg";

        if (!cutscene4Texture[i].loadFromFile(filename.str())) {
            cerr << "Erreur lors du chargement de l'image " << filename.str() << endl;
        }
    }
}

Cutscene::~Cutscene() {
    cout << "Le menu settings est détruit\n";
}

void Cutscene::draw(RenderWindow& window) {
    //SoundManager& soundManager = SoundManager::getInstance();
    float cutscene2CooldownTime = 0.05f;
    if (cooldownframe.getElapsedTime().asSeconds() >= cutscene2CooldownTime) {
        currentFrame2++;
        cutscene2Sprite.setTexture(cutscene2Texture[currentFrame2]);
        cooldownframe.restart();
    }
    window.draw(cutscene2Sprite);
}
void Cutscene::draw1(RenderWindow& window) {
    //SoundManager& soundManager = SoundManager::getInstance();
    float cutscene1CooldownTime = 0.05f;
    if (cooldownframe.getElapsedTime().asSeconds() >= cutscene1CooldownTime) {
        currentFrame1++;
        cutscene1Sprite.setTexture(cutscene1Texture[currentFrame1]);
        cooldownframe.restart();
    }
    window.draw(cutscene1Sprite);
}
void Cutscene::draw2(RenderWindow& window) {
    //SoundManager& soundManager = SoundManager::getInstance();
    float cutscene3CooldownTime = 0.05f;
    if (cooldownframe.getElapsedTime().asSeconds() >= cutscene3CooldownTime) {
        currentFrame3++;
        cutscene3Sprite.setTexture(cutscene3Texture[currentFrame3]);
        cooldownframe.restart();
    }
    window.draw(cutscene3Sprite);
}
void Cutscene::draw3(RenderWindow& window) {
    //SoundManager& soundManager = SoundManager::getInstance();
    float cutscene4CooldownTime = 0.05f;
    if (cooldownframe.getElapsedTime().asSeconds() >= cutscene4CooldownTime) {
        currentFrame4++;
        cutscene4Sprite.setTexture(cutscene4Texture[currentFrame4]);
        cooldownframe.restart();
    }
    window.draw(cutscene4Sprite);
}
