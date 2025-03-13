#include "../include/Game.hpp"
#include "../include/Settings.hpp"
#include "../include/Map.hpp"
#include "../include/Menu.hpp"
#include "../include/Pause.hpp"
#include "../include/Player.hpp"
#include "../include/Enemy.hpp"
#include "../include/RangedEnemy.hpp"
#include "../include/TileSelector.hpp"
#include "../include/EnemyFlying.hpp"
#include "../include/ChargingBoss.hpp"
#include "../include/Background.hpp"

#include <iostream>

using namespace sf;
using namespace std;

enum class GameState {
    Menu,
    Playing,
    Editor,
    Pause,
    Settings,
    GameOver
};

Game::Game() : deltaTime(0.0f) {}

Game::~Game() {
    cout << "Le jeu est détruit\n";
}

void Game::run() {
    RenderWindow window(VideoMode(1920, 1080), "Map Editor");
    window.setFramerateLimit(60);

    background.loadTextures("assets/background/layer1.png",
        "assets/background/layer2.png",
        "assets/background/layer3.png",
        "assets/background/layer4.png");

    foreground.loadTextures("assets/foreground/layer1.png",
        "assets/foreground/layer2.png");
    GameState currentState = GameState::Menu;

    // Instanciation des objets
    Map level1("assets/tileset/tileset_green.png", "assets/map/Lobby.txt");
    Map map("assets/tileset/tileset_green.png", "assets/map/Lobby.txt");
    Menu menu;
    Settings settings;
    Pause pause;
    TileSelector tileSelector("assets/tileset/tileset_green.png", 64);
    Player player(Vector2f(50, 50), Color::Red, map);
    Enemy enemy(Vector2f(50, 50), Color::Blue, map);
    RangedEnemy rangedEnemy(Vector2f(50, 50), Color::Yellow);
    EnemyFlying flyingEnemy(Vector2f(50, 50), Color::Green);
    ChargingBoss chargingBoss(Vector2f(100, 100), Color(239, 12, 197), map);

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
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (menu.settingSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Settings;
                    }
                }
                break;

            case GameState::Playing:
                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    window.setView(window.getDefaultView());
                    currentState = GameState::Pause;
                }

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
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (pause.playSpritePause.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Playing;
                    }
                    if (pause.menuSpritePause.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Menu;
                    }

                }
                break;

            case GameState::GameOver:
                break;
            }
        }

        if (currentState == GameState::Playing || currentState == GameState::Editor) {
            player.update(deltaTime);

            player.handleInput(event, window, deltaTime);
            rangedEnemy.update(deltaTime);
            enemy.update(0.016f);
            flyingEnemy.update(deltaTime, player);
            float cameraX = player.getSprite().getPosition().x;
            background.update(cameraX);
            foreground.update(cameraX);
            chargingBoss.behavior(deltaTime, player, window);
        }

        window.clear();

        switch (currentState) {
        case GameState::Menu:
            menu.draw(window);
            break;

        case GameState::Playing:
            background.draw(window);
            level1.draw(window);
            player.draw(window);
            enemy.draw(window);
            rangedEnemy.draw(window);
            rangedEnemy.drawProjectiles(window);
            flyingEnemy.draw(window);
            foreground.draw(window);
            chargingBoss.draw(window);

            break;
        case GameState::Settings:
            settings.draw(window);
            break;
        case GameState::Editor:
            background.draw(window);
            map.draw(window);
			map.drawCam(window);
            tileSelector.draw(window);
            foreground.draw(window);
            break;

        case GameState::Pause:
            pause.draw(window);
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
