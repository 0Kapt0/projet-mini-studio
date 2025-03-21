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
