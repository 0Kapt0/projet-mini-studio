#include <SFML/Graphics.hpp>
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"
#include "../include/RangedEnemy.hpp"

using namespace sf;
using namespace std;

int main() 
{
    RenderWindow window(VideoMode(1440, 1080), "Grapple Example");
	Enemy enemy = Enemy(Vector2f(50, 50), Color::Blue);
    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");

    Player player(Vector2f(50, 50), Color::Red, map);

    RangedEnemy rangedenemy(Vector2f(50, 50), Color::Yellow);

    window.setFramerateLimit(60);

    Clock clock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            player.handleInput(event, window);

            if (event.type == Event::MouseButtonPressed) {
                int tileIndex = 84;
                map.handleClick(event.mouseButton.x, event.mouseButton.y, tileIndex);
            }
        }

        float deltaTime = clock.restart().asSeconds();

        player.update(deltaTime);

        rangedenemy.update(deltaTime);
		enemy.update(0.016f);
        // Effacer la fenêtre
        window.clear();
        map.draw(window);
		player.draw(window);
		enemy.draw(window);
        rangedenemy.draw(window);
		rangedenemy.drawProjectiles(window);
        // Afficher le contenu
        window.display();
    }

    map.saveMap("assets/map/Lobby.txt");

    return 0;
}

