#include <SFML/Graphics.hpp>
#include "../include/Map.hpp"

using namespace sf;
using namespace std;

int main() {
    RenderWindow window(VideoMode(1080, 1080), "Map Editor");

    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed) {
                int tileIndex = 50;
                map.handleClick(event.mouseButton.x, event.mouseButton.y, tileIndex);
            }
        }

        window.clear();
        map.draw(window);
        window.display();
    }

    map.saveMap("assets/map/Lobby.txt");

    return 0;
}
