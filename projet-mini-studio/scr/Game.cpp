#include "../include/Game.hpp"
#include "../include/Map.hpp"
#include "../include/Menu.hpp"
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"
#include "../include/RangedEnemy.hpp"
#include "../include/TileSelector.hpp"

#include <iostream>

using namespace sf;
using namespace std;

enum class GameState {
    Menu,
    Playing,
    Editor,
    Pause,
    GameOver
};

Game::Game() {}

Game::~Game() {
    cout << "Le jeu est détruit\n";
}

void Game::run() {
    RenderWindow window(VideoMode(1920, 1080), "Map Editor");
    window.setFramerateLimit(60);

    // Définir l'état initial du jeu
    GameState currentState = GameState::Menu;

    // Instanciation des objets
    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");
    Map level1("assets/tileset/Tileset_Grass.png", "assets/map/Level1.txt");
    Menu menu;
    TileSelector tileSelector("assets/tileset/Tileset_Grass.png", 32);
    Player player(Vector2f(50, 50), Color::Red, map);
    Enemy enemy(Vector2f(50, 50), Color::Blue, map);
    RangedEnemy rangedEnemy(Vector2f(50, 50), Color::Yellow);

    bool collisionMode = false;
    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            // Gestion des événements en fonction de l'état actuel
            switch (currentState) {
            case GameState::Menu:
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (menu.editSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Editor;
                    }
                }
               if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (menu.playSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Playing;
                    }
                }
                break;

            case GameState::Playing:
                player.handleInput(event, window);
                break;

            case GameState::Editor:
                player.handleInput(event, window);
                tileSelector.handleEvent(event, window);

                if (event.type == Event::MouseButtonPressed) {
                    int selectedTile = tileSelector.getSelectedTile();
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;

                    // Clic gauche : placer une tuile
                    if (event.mouseButton.button == Mouse::Left && selectedTile != -1) {
                        map.handleClick(x, y, selectedTile);
                    }
                    // Clic droit : effacer une tuile
                    else if (event.mouseButton.button == Mouse::Right) {
                        map.handleClick(x, y, 0);
                    }
                }

                // Touche "C" pour activer/désactiver la collision
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::C) {
                    tileSelector.toggleCollision();
                }
                break;

            case GameState::Pause:
                break;

            case GameState::GameOver:
                break;
            }
        }

        // Gestion du temps
        float deltaTime = clock.restart().asSeconds();

        // Mise à jour selon l'état du jeu
        if (currentState == GameState::Playing || currentState == GameState::Editor) {
            player.update(deltaTime);
            rangedEnemy.update(deltaTime);
            enemy.update(0.016f);
        }

        window.clear();

        // Rendu en fonction de l'état du jeu
        switch (currentState) {
        case GameState::Menu:
            menu.draw(window);
            break;

        case GameState::Playing:
            level1.draw(window);
            player.draw(window);
            enemy.draw(window);
            rangedEnemy.draw(window);
            rangedEnemy.drawProjectiles(window);
            break;

        case GameState::Editor:
            map.draw(window);
            tileSelector.draw(window);
            player.draw(window);
            enemy.draw(window);
            rangedEnemy.draw(window);
            rangedEnemy.drawProjectiles(window);
            break;

        case GameState::Pause:
            break;

        case GameState::GameOver:
            break;
        }

        window.display();
    }

    // Sauvegarde de la carte en quittant l'éditeur
    if (currentState == GameState::Editor) {
        map.saveMap("assets/map/Lobby.txt");
    }
}
