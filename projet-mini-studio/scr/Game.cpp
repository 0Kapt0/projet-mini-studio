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

Game::Game() : deltaTime(0.0f) {}

Game::~Game() {
    cout << "Le jeu est détruit\n";
}

void Game::run() {
    RenderWindow window(VideoMode(1920, 1080), "Map Editor");
    window.setFramerateLimit(60);

    GameState currentState = GameState::Menu;

    // Instanciation des objets
    Map level1("assets/tileset/Tileset_Grass.png", "assets/map/Level1.txt");
    Map map("assets/tileset/tileset_green.png", "assets/map/Lobby.txt");
    Menu menu;
    TileSelector tileSelector("assets/tileset/tileset_green.png", 64);
    Player player(Vector2f(50, 50), Color::Red, map);
    Enemy enemy(Vector2f(50, 50), Color::Blue);
    RangedEnemy rangedEnemy(Vector2f(50, 50), Color::Yellow);

    bool collisionMode = false;
    Clock clock;

    while (window.isOpen()) {
        Event event;
		deltaTime = clock.restart().asSeconds();
        bool isLeftMousePressed = false;
        bool isRightMousePressed = false;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    isLeftMousePressed = true;
                }
                if (event.mouseButton.button == Mouse::Right) {
                    isRightMousePressed = true;
                }
            }

            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    isLeftMousePressed = false;
                }
                if (event.mouseButton.button == Mouse::Right) {
                    isRightMousePressed = false;
                }
            }


            //Gestion des événements en fonction de l'état actuel
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
                player.update(deltaTime);
                rangedEnemy.update(deltaTime);
                enemy.update(0.016f);


                break;

            case GameState::Editor:
                tileSelector.handleEvent(event, window);
				map.handleEvent(event);

                if (event.type == Event::MouseButtonPressed) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    Vector2f viewPos = window.getView().getCenter() - (window.getView().getSize() / 2.f);

                    int tilesetWidth = tileSelector.getTilesetWidth();
                    int tilesetHeight = tileSelector.getTilesetHeight();
                    int tileSize = tileSelector.getTileSize();

                    FloatRect tileSelectorBounds(
                        viewPos.x, viewPos.y,
                        tilesetWidth * tileSize,
                        tilesetHeight * tileSize
                    );

                    if (tileSelectorBounds.contains(worldPos)) {
                        tileSelector.handleEvent(event, window);
                    }
                    else {
                        int selectedTile = tileSelector.getSelectedTile();
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;

                        //Clic gauche : placer une tuile
                        if (event.mouseButton.button == Mouse::Left && selectedTile != -1) {
                            map.handleClick(window, x, y, selectedTile);
                        }
                        //Clic droit : effacer une tuile
                        else if (event.mouseButton.button == Mouse::Right) {
                            map.handleClick(window, x, y, 74);
                        }
                    }
                }


                //Touche "C" pour activer/désactiver la collision (ne fonctionne pas pour l'instant)
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

        if (currentState == GameState::Playing) {
            
        }

        window.clear();

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
