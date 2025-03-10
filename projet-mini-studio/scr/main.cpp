#include <SFML/Graphics.hpp>
#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"
#include "../include/RangedEnemy.hpp"
#include "../include/TileSelector.hpp"


using namespace sf;
using namespace std;

int main() 
{
    RenderWindow window(VideoMode(1440, 1080), "Grapple Example");
	Enemy enemy = Enemy(Vector2f(50, 50), Color::Blue);
    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");
    TileSelector tileSelector("assets/tileset/Tileset_Grass.png", 32);

    bool collisionMode = false;

    Player player(Vector2f(50, 50), Color::Red, map);

    RangedEnemy rangedenemy(Vector2f(50, 50), Color::Yellow);

    window.setFramerateLimit(60);

    Clock clock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            tileSelector.handleEvent(event, window);
            player.handleInput(event, window);

            if (event.type == Event::MouseButtonPressed) {
                int selectedTile = tileSelector.getSelectedTile();
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;

                //Clic gauche : placer une tuile
                if (event.mouseButton.button == Mouse::Left && selectedTile != -1) {
                    map.handleClick(x, y, selectedTile);
                }
                //Clic droit : effacer une tuile
                else if (event.mouseButton.button == Mouse::Right) {
                    map.handleClick(x, y, 0);
                }
            }

            //Touche "C" toogle collision
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::C) {
                tileSelector.toggleCollision();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        player.update(deltaTime);

        rangedenemy.update(deltaTime);
		enemy.update(0.016f);
        window.clear();
        map.draw(window);
        tileSelector.draw(window);
		player.draw(window);
		enemy.draw(window);
        rangedenemy.draw(window);
		rangedenemy.drawProjectiles(window);
        window.display();
    }
int main() {
    Game game;

    game.run();

    return 0;
}

