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
#include "../include/BasicEnemy.hpp"
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
    cout << "Le jeu est d�truit\n";
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
    RangedEnemy rangedEnemy(Vector2f(50, 50), Color::Yellow, map);
    EnemyFlying flyingEnemy(Vector2f(50, 50), Color::Green, map);
	BasicEnemy basicEnemy(Vector2f(50, 50), Color::Blue,map);
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

            //Gestion des �v�nements en fonction de l'�tat actuel
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
            {
                tileSelector.handleEvent(event, window);
                map.handleEvent(event);

                Vector2i mousePos = Mouse::getPosition(window);
                Vector2f worldPos = window.mapPixelToCoords(mousePos);

                float tilesetWidth = tileSelector.getTilesetWidth() * tileSelector.getTileSize();
                float tilesetHeight = tileSelector.getTilesetHeight() * tileSelector.getTileSize();
                Vector2f viewPos = window.getView().getCenter() - (window.getView().getSize() / 2.f);

                FloatRect tileSelectorBounds(viewPos.x, viewPos.y, tilesetWidth, tilesetHeight);

                bool leftPressed = Mouse::isButtonPressed(Mouse::Left);
                bool rightPressed = Mouse::isButtonPressed(Mouse::Right);

                if (!tileSelectorBounds.contains(worldPos)) {
                    if (leftPressed) {
                        int selectedTile = tileSelector.getSelectedTile();
                        if (selectedTile != -1) {
                            map.handleClick(window, mousePos.x, mousePos.y, selectedTile);
                        }
                    }
                    else if (rightPressed) {
                        map.handleClick(window, mousePos.x, mousePos.y, 74);
                    }
                }

                break;
            }


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

        if (currentState == GameState::Playing) {
            player.update(deltaTime);

            player.handleInput(event, window, deltaTime);
            rangedEnemy.update(deltaTime);
			basicEnemy.update(deltaTime, player);
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
			basicEnemy.draw(window);
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
            map.drawGrid(window);
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

    // Sauvegarde de la carte en quittant l'�diteur
    if (currentState == GameState::Editor) {
        map.saveMap("assets/map/Lobby.txt");
    }
}
