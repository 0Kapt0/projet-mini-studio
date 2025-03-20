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

#include <iostream>
#include <string>
#include <array>

using namespace sf;
using namespace std;

struct LevelAssets {
    array<string, 6> backgroundTextures;
    array<string, 2> foregroundTextures;
    string mapTileset;
    string tileSelectorTileset;
};

// Exemple d'association entre le fichier de niveau et les assets correspondants
map<string, LevelAssets> levelAssetsMap = {
    {"assets/map/Level1.txt", {
        { "assets/background/Forest1.png",
          "assets/background/Forest2.png",
          "assets/background/Forest3.png",
          "assets/background/Forest4.png",
          "assets/background/Forest5.png",
          "assets/background/Forest6.png" },
        { "assets/foreground/foret_foreground.png",
          "assets/foreground/Forest-light.png" },
        "assets/tileset/tileset_green_vFinal.png",
        "assets/tileset/tileset_green_vFinal.png"
    }},
    {"assets/map/Level2.txt", {
        { "assets/background/BG1.png",
          "assets/background/BG2.png",
          "assets/background/BG3.png",
          "assets/background/BG4.png",
          "assets/background/BG5.png",
          "assets/background/BG6.png" },
        { "assets/foreground/foreground_city.png",
          "assets/foreground/level2_foreground2.png" },
        "assets/tileset/tilesetTownV1.png",
        "assets/tileset/tilesetTownV1.png"
    }}
};

void setLevel(const string& levelFile,
    Background& background,
    Foreground& foreground,
    Map& map,
    TileSelector& tileSelector) {
    if (levelAssetsMap.find(levelFile) != levelAssetsMap.end()) {
        LevelAssets assets = levelAssetsMap[levelFile];

        if (!background.loadTextures(assets.backgroundTextures[0],
            assets.backgroundTextures[1],
            assets.backgroundTextures[2],
            assets.backgroundTextures[3],
            assets.backgroundTextures[4],
            assets.backgroundTextures[5])) {
            std::cerr << "Erreur de chargement du background pour " << levelFile << endl;
        }

        if (!foreground.loadTextures(assets.foregroundTextures[0],
            assets.foregroundTextures[1])) {
            std::cerr << "Erreur de chargement du foreground pour " << levelFile << endl;
        }

        map.setTileset(assets.mapTileset);
        map.generateTiles();

        tileSelector.setTileset(assets.tileSelectorTileset);

        map.loadMap(levelFile);
        map.generateTiles();

        std::cout << "Niveau " << levelFile << " chargé avec succès." << endl;
    }
    else {
        std::cerr << "Niveau inconnu: " << levelFile << endl;
    }
}

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

    Background background;
    Foreground foreground;

    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        cerr << "Erreur chargement de la police.\n";
    }

    vector<string> mapFiles = {
        "assets/map/Level1.txt",
        "assets/map/Level2.txt",
        "assets/map/Level3.txt"
    };

    DropDown mapDropdown(font, mapFiles,
        Vector2f(1600.f, 80.f),
        Vector2f(250.f, 30.f));

    string currentLevelFile = "assets/map/Level1.txt";
    EnemySelector enemySelector("assets/tileset/enemy_icons.png", 64);
    Map map("assets/tileset/tilesetTownV1.png", "assets/map/Lobby.txt");
    TileSelector tileSelector("assets/tileset/tilesetTownV1.png", 64);
    setLevel("assets/map/Level1.txt", background, foreground, map, tileSelector);

    EntityManager entityManager;
    entityManager.createEntity("Player", Vector2f(200, 200), Vector2f(50, 50), Color::Red, map);

    bool collisionMode = false;
    Clock clock;

    GameState currentState = GameState::Menu;
    Menu menu;
    Settings settings;
    Pause pause;

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
                        map.saveMap(currentLevelFile);
                        currentLevelFile = chosenFile;
                        setLevel(currentLevelFile, background, foreground, map, tileSelector);
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

        window.clear();

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
        map.saveMap(currentLevelFile);
    }
}
