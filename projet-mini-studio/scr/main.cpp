#include <SFML/Graphics.hpp>
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"

int main() {
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fenêtre SFML");

	Player player = Player(Vector2f(50, 50), Color::Red);
	Enemy enemy = Enemy(Vector2f(50, 50), Color::Blue);

    window.setFramerateLimit(60);

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close(); // Fermer la fenêtre
        }
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