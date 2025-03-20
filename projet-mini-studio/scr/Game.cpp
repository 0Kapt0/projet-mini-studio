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
#include "../include/EntityManager.hpp"
#include "../include/DropDown.hpp"
#include "../include/EnemySelector.hpp"
#include "../include/SoundManager.hpp"

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

    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        cerr << "Erreur chargement de la police.\n";
    }

    vector<string> mapFiles = {
        "assets/map/Lobby.txt",
        "assets/map/Level1.txt",
        "assets/map/Level2.txt"
    };

    DropDown mapDropdown(font, mapFiles,
        Vector2f(1600.f, 80.f),
        Vector2f(250.f, 30.f));

    EnemySelector enemySelector("assets/tileset/enemy_icons.png", 64);

    background.loadTextures("assets/background/city1.png",
        "assets/background/city2.png",
        "assets/background/city3.png",
        "assets/background/city4.png",
        "assets/background/city5.png");

    /*foreground.loadTextures("assets/foreground/foret_foreground.png", "assets/foreground/Forest-light.png");*/
    GameState currentState = GameState::Menu;

    // Instanciation des objets
    /*Map level1("assets/tileset/tileset_green.png", "assets/map/Lobby.txt");*/
    Map map("assets/tileset/tileset_green_vFinal.png", "assets/map/Lobby.txt");
    Menu menu;
    Settings settings;
    Pause pause;
    TileSelector tileSelector("assets/tileset/tileset_green_vFinal.png", 64);
    EntityManager entityManager;
	SoundManager soundManager;
    entityManager.createEntity("Player", Vector2f(200, 200), Vector2f(50, 50), Color::Red, map);
    entityManager.createEntity("RangedEnemy", Vector2f(0, 0), Vector2f(50, 50), Color::Yellow, map);
    entityManager.createEntity("EnemyFlying", Vector2f(0, 0), Vector2f(50, 50), Color::Green, map);
    entityManager.createEntity("BasicEnemy", Vector2f(0, 0), Vector2f(50, 50), Color::Blue, map);
    entityManager.createEntity("ChargingBoss", Vector2f(500, 800), Vector2f(100, 100), Color(239, 12, 197), map);

    // Instanciation des sons
	soundManager.loadSound("Level1Music", "assets/sfx/Level1Music.mp3");
    soundManager.loadSound("Level2Music", "assets/sfx/Level2Music.mp3");
    soundManager.loadSound("Level3Music", "assets/sfx/Level3Music.mp3");
    soundManager.loadSound("BonusColectedSound", "assets/sfx/BonusColectedSound.mp3");
    soundManager.loadSound("BossDash1", "assets/sfx/BossDash1.mp3");
    soundManager.loadSound("BossSound1", "assets/sfx/BossSound1.mp3");
    soundManager.loadSound("CapsuleOppening", "assets/sfx/CapsuleOppening.mp3");
    soundManager.loadSound("Checkpoint", "assets/sfx/Checkpoint.mp3");
    soundManager.loadSound("CinematicBoss1", "assets/sfx/CinematicBoss1.mp3");
    soundManager.loadSound("CounterAttack", "assets/sfx/CounterAttack.mp3");
    soundManager.loadSound("DamageSound", "assets/sfx/DamageSound.mp3");
    soundManager.loadSound("Dash", "assets/sfx/Dash.mp3");
    soundManager.loadSound("EnemyWalking", "assets/sfx/EnemyWalking.mp3");
    soundManager.loadSound("FlyingRobotSound", "assets/sfx/FlyingRobotSound.mp3");
    soundManager.loadSound("ForestWindSound", "assets/sfx/ForestWindSound.mp3");
    soundManager.loadSound("GrapleShootSound", "assets/sfx/GrapleShootSound.mp3");
    soundManager.loadSound("ItemColectedSound", "assets/sfx/ItemColectedSound.mp3");
    soundManager.loadSound("JumpingSound", "assets/sfx/JumpingSound.mp3");
    soundManager.loadSound("MeleeAttackSound", "assets/sfx/MeleeAttackSound.mp3");
    soundManager.loadSound("RespawnSound", "assets/sfx/RespawnSound.mp3");
    soundManager.loadSound("RobotDeathSound", "assets/sfx/RobotDeathSound.mp3");
    soundManager.loadSound("RunningSound", "assets/sfx/RunningSound.mp3");
    soundManager.loadSound("TakingHitSound", "assets/sfx/TakingHitSound.mp3");
    soundManager.loadSound("WindSound", "assets/sfx/WindSound.mp3");
    soundManager.loadSound("MainTitle", "assets/sfx/MainTitle.mp3");

    bool collisionMode = false;
    Clock clock;

    while (window.isOpen()) {
        window.clear();
        Event event;
		deltaTime = clock.restart().asSeconds();
        bool isLeftMousePressed = false;
        bool isRightMousePressed = false;

        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed)
                window.close();

            //Gestion des événements en fonction de l'état actuel
            switch (currentState)
            {
            case GameState::Menu:
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) 
                {
                    if (menu.editSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) 
                    {
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
                /*map.loadMap("assets/map/Lobby.txt");
                map.generateTiles();*/
                if (!enemiesGenerated) {
                    entityManager.generateEnemies(map);
                    enemiesGenerated = true;
                }

                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    window.setView(window.getDefaultView());
                    currentState = GameState::Pause;
                }

                break;

            case GameState::Editor:
            {
                tileSelector.handleEvent(event, window);
                enemySelector.handleEvent(event, window);
                map.handleEvent(event, window);
                mapDropdown.handleEvent(event, window);

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                    string chosenFile = mapDropdown.getSelectedItem();
                    if (!chosenFile.empty()) {
                        map.loadMap(chosenFile);
                        map.generateTiles();
                    }
                }

                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    window.setView(window.getDefaultView());
                    currentState = GameState::Pause;
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::G) {
                    showGrid = !showGrid;
                }

                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Z) {
                    bool ctrl = event.key.control;
                    bool shift = event.key.shift;
                    if (ctrl && !shift)
                        map.undo();
                    else if (ctrl && shift)
                        map.redo();
                }

                if (event.type == Event::MouseButtonReleased) {
                    if (event.mouseButton.button == Mouse::Left ||
                        event.mouseButton.button == Mouse::Right)
                    {
                        if (!map.currentDragChanges.empty()) {
                            map.pushAction(map.currentDragChanges);
                            map.currentDragChanges.clear();
                        }
                    }
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::E) {
                        currentPlacementMode = PlacementMode::Enemies;
                    }
                    else if (event.key.code == Keyboard::T) {
                        currentPlacementMode = PlacementMode::Tiles;
                    }
                }

                if (event.type == Event::MouseButtonPressed) {
                    if (event.mouseButton.button == Mouse::Left) {
                        Vector2i mousePos = Mouse::getPosition(window);
                        FloatRect tileSelectorBounds(0.f, 0.f,
                            tileSelector.getTilesetWidth() * tileSelector.getTileSize(),
                            tileSelector.getTilesetHeight() * tileSelector.getTileSize());
                        FloatRect enemySelectorBounds(1500.f, 0.f, 256.f, 64.f);

                        if (!tileSelectorBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) &&
                            !enemySelectorBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                        {
                            if (currentPlacementMode == PlacementMode::Enemies) {
                                EnemyType eType = enemySelector.getSelectedEnemy();
                                if (eType != EnemyType::None) {
                                    map.handleEnemyPlacement(window, mousePos.x, mousePos.y, eType);
                                }
                            }
                            else if (currentPlacementMode == PlacementMode::Tiles) {
                                int selectedTile = tileSelector.getSelectedTile();
                                if (selectedTile != -1) {
                                    map.handleClick(window, mousePos.x, mousePos.y, selectedTile);
                                }
                            }
                        }
                    }
                    else if (event.mouseButton.button == Mouse::Right) {
                        Vector2i mousePos = Mouse::getPosition(window);
                        FloatRect tileSelectorBounds(0.f, 0.f,
                            tileSelector.getTilesetWidth() * tileSelector.getTileSize(),
                            tileSelector.getTilesetHeight() * tileSelector.getTileSize());
                        FloatRect enemySelectorBounds(1500.f, 0.f, 256.f, 64.f);
                        if (!tileSelectorBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) &&
                            !enemySelectorBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                        {
                            if (currentPlacementMode == PlacementMode::Enemies) {
                                map.handleEnemyRemoval(window, mousePos.x, mousePos.y);
                            }
                            else if (currentPlacementMode == PlacementMode::Tiles) {
                                map.handleClick(window, mousePos.x, mousePos.y, 74);
                            }
                        }
                    }
                }

                {
                    Vector2i mousePos = Mouse::getPosition(window);
                    bool leftHeld = Mouse::isButtonPressed(Mouse::Left);
                    bool rightHeld = Mouse::isButtonPressed(Mouse::Right);
                    if (currentPlacementMode == PlacementMode::Tiles) {
                        if (leftHeld) {
                            int selectedTile = tileSelector.getSelectedTile();
                            if (selectedTile != -1) {
                                map.handleClick(window, mousePos.x, mousePos.y, selectedTile);
                            }
                        }
                        else if (rightHeld) {
                            map.handleClick(window, mousePos.x, mousePos.y, 74);
                        }
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
            entityManager.updateEntities(event, deltaTime, window);
            entityManager.destroyEntity();
            float cameraX = entityManager.player->getSprite().getPosition().x;
            background.update(cameraX);
            foreground.update(cameraX);
        }

        switch (currentState) {
        case GameState::Menu:
            menu.draw(window);
            break;

        case GameState::Playing:
            background.draw(window);
            map.draw(window);;
            entityManager.drawEntities(window);
            foreground.draw(window);
            break;
        case GameState::Settings:
            settings.draw(window);
            break;
        case GameState::Editor:
            background.draw(window);
            foreground.draw(window);
            map.draw(window);
            if (showGrid) {
                map.drawGrid(window);
            }
            map.drawEnemySpawns(window);

			map.drawCam(window);
            tileSelector.draw(window);
            enemySelector.draw(window);
            

            oldView = window.getView();
            window.setView(window.getDefaultView());
            mapDropdown.draw(window);
            window.setView(oldView);
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
    if (currentState == GameState::Editor || currentState == GameState::Menu || (currentState == GameState::Playing)) {
        map.saveMap("assets/map/Lobby.txt");
    }
}
