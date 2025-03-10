#include <SFML/Graphics.hpp>
#include "../include/Player.hpp"

int main() {
    RenderWindow window(VideoMode(1440, 1080), "Grapple Example");
    Player player(Vector2f(50, 50), Color::Red); // Exemple de taille et couleur du joueur

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

        window.clear();
        player.draw(window);
        window.display();
    }

    return 0;
}

