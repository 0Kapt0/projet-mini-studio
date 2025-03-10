#include <SFML/Graphics.hpp>
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"

int main() {
    RenderWindow window(VideoMode(1440, 1080), "Grapple Example");
    Player player(Vector2f(50, 50), Color::Red); // Exemple de taille et couleur du joueur

	Player player = Player(Vector2f(50, 50), Color::Red);
	Enemy enemy = Enemy(Vector2f(50, 50), Color::Blue);

    window.setFramerateLimit(60);

    Clock clock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            player.handleInput(event, window);
        }

        float deltaTime = clock.restart().asSeconds();

        player.update(deltaTime);

		player.update(0.016f);
		enemy.update(0.016f);
        // Effacer la fenêtre
        window.clear();

		player.draw(window);
		enemy.draw(window);
        // Afficher le contenu
        window.display();
    }

    return 0;
}

