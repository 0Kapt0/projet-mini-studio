#include <SFML/Graphics.hpp>
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"

int main() {
    // Cr�ation de la fen�tre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fen�tre SFML");

	Player player = Player(Vector2f(50, 50), Color::Red);
	Enemy enemy = Enemy(Vector2f(50, 50), Color::Blue);

    window.setFramerateLimit(60);

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Fermer la fen�tre
        }
		player.update(0.016f);
		enemy.update(0.016f);
        // Effacer la fen�tre
        window.clear();

		player.draw(window);
		enemy.draw(window);
        // Afficher le contenu
        window.display();
    }

    return 0;
}