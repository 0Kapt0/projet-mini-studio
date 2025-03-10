#include <SFML/Graphics.hpp>
#include "../include/Map.hpp"

using namespace sf;
using namespace std;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Map Editor");

    // Créer un objet Map avec le chemin du tileset et du fichier de carte
    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed) {
                int tileIndex = 1; // Index de la tuile choisie (modifiable via une interface)
                map.handleClick(event.mouseButton.x, event.mouseButton.y, tileIndex);
            }
        }
        window.clear();
        map.draw(window);  // Dessiner la carte
        window.display();
    }
    // Sauvegarder la carte après avoir quitté
    map.saveMap("map.txt");

    return 0;
}
