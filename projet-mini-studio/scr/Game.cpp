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
        Vector2f(1600.f, 80.f),   //position à l'écran
        Vector2f(250.f, 30.f)); //taille (largeur, hauteur

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
    Map map("assets/tileset/tileset_green.png", "assets/map/Lobby.txt");
    Menu menu;
    Settings settings;
    Pause pause;
    TileSelector tileSelector("assets/tileset/tileset_green.png", 64);
    /*Player player(Vector2f(50, 50), Color::Red, map);
    RangedEnemy rangedEnemy(Vector2f(50, 50), Color::Yellow, map);
    EnemyFlying flyingEnemy(Vector2f(50, 50), Color::Green, map);
	BasicEnemy basicEnemy(Vector2f(50, 50), Color::Blue,map);
    ChargingBoss chargingBoss(Vector2f(100, 100), Color(239, 12, 197), map);*/
    EntityManager entityManager;
    entityManager.createEntity("Player", Vector2f(200, 200), Vector2f(50, 50), Color::Red, map);
    entityManager.createEntity("RangedEnemy", Vector2f(0, 0), Vector2f(50, 50), Color::Yellow, map);
    entityManager.createEntity("EnemyFlying", Vector2f(0, 0), Vector2f(50, 50), Color::Green, map);
    entityManager.createEntity("BasicEnemy", Vector2f(0, 0), Vector2f(50, 50), Color::Blue, map);
    entityManager.createEntity("ChargingBoss", Vector2f(500, 800), Vector2f(100, 100), Color(239, 12, 197), map);



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
                /*map.loadMap("assets/map/Lobby.txt");
                map.generateTiles();*/
                if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                    window.setView(window.getDefaultView());
                    currentState = GameState::Pause;
                }

                break;

            case GameState::Editor:
            {
                // --- GESTION DES ÉVÉNEMENTS (pollEvent) ---
                tileSelector.handleEvent(event, window);
                enemySelector.handleEvent(event, window);
                map.handleEvent(event, window);
                mapDropdown.handleEvent(event, window);

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    string chosenFile = mapDropdown.getSelectedItem();
                    if (!chosenFile.empty()) {
                        map.loadMap(chosenFile);
                        map.generateTiles();
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    window.setView(window.getDefaultView());
                    currentState = GameState::Pause;
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
                    showGrid = !showGrid;
                }

                // Undo / Redo
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
                    bool ctrl = event.key.control;
                    bool shift = event.key.shift;
                    if (ctrl && !shift)
                        map.undo();
                    else if (ctrl && shift)
                        map.redo();
                }

                // Finaliser le drag des tuiles lors du relâchement de la souris
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left ||
                        event.mouseButton.button == sf::Mouse::Right)
                    {
                        if (!map.currentDragChanges.empty()) {
                            map.pushAction(map.currentDragChanges);
                            map.currentDragChanges.clear();
                        }
                    }
                }

                // --- Placement unique lors d'un clic ---
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    // Convertir la position avec la vue de la carte
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, map.getCameraView());
                    // Snap à la grille
                    int tileX = static_cast<int>(worldPos.x) / Map::TILE_SIZE;
                    int tileY = static_cast<int>(worldPos.y) / Map::TILE_SIZE;
                    worldPos.x = tileX * Map::TILE_SIZE;
                    worldPos.y = tileY * Map::TILE_SIZE;

                    // Définir des zones pour éviter que le clic ne touche les palettes
                    sf::FloatRect tileSelectorBounds(0.f, 0.f, tileSelector.getTilesetWidth() * tileSelector.getTileSize(),
                        tileSelector.getTilesetHeight() * tileSelector.getTileSize());
                    sf::FloatRect enemySelectorBounds(0.f, 0.f, 256.f, 64.f);  // À adapter selon votre configuration

                    if (!tileSelectorBounds.contains((float)mousePos.x, (float)mousePos.y) &&
                        !enemySelectorBounds.contains((float)mousePos.x, (float)mousePos.y))
                    {
                        // Si un ennemi est sélectionné, placer un ennemi (unique)
                        EnemyType eType = enemySelector.getSelectedEnemy();
                        if (eType != EnemyType::None) {
                            string typeStr;
                            switch (eType) {
                            case EnemyType::Flying:        typeStr = "EnemyFlying"; break;
                            case EnemyType::Ranged:        typeStr = "RangedEnemy"; break;
                            case EnemyType::Basic:         typeStr = "BasicEnemy";  break;
                            case EnemyType::ChargingBoss:  typeStr = "ChargingBoss"; break;
                            default:                       typeStr = "EnemyFlying"; break;
                            }
                            // Vous pouvez directement enregistrer le spawn sans créer l'entité ici,
                            // ou créer l'entité si vous le souhaitez pour la prévisualisation.
                            EnemySpawn spawn;
                            spawn.type = typeStr;
                            spawn.x = worldPos.x;
                            spawn.y = worldPos.y;
                            map.enemySpawns.push_back(spawn);
                        }
                        else {
                            // Sinon, placement d'une tuile (unique)
                            int selectedTile = tileSelector.getSelectedTile();
                            if (selectedTile != -1) {
                                map.handleClick(window, mousePos.x, mousePos.y, selectedTile);
                            }
                        }
                    }
                }

                // --- Drag pour les tuiles (peindre/effacer en continu) ---
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, map.getCameraView());
                    bool leftHeld = sf::Mouse::isButtonPressed(sf::Mouse::Left);
                    bool rightHeld = sf::Mouse::isButtonPressed(sf::Mouse::Right);

                    // Si aucun ennemi n'est sélectionné, on fait du drag sur les tuiles
                    if (enemySelector.getSelectedEnemy() == EnemyType::None) {
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
            /*player.update(deltaTime);

            player.handleInput(event, window, deltaTime);
            rangedEnemy.update(deltaTime);
			basicEnemy.update(deltaTime, player);
            flyingEnemy.update(deltaTime, player);*/
            entityManager.updateEntities(event, deltaTime, window);
            entityManager.destroyEntity();
            float cameraX = entityManager.player->getSprite().getPosition().x;
            background.update(cameraX);
            foreground.update(cameraX);
            /*chargingBoss.behavior(deltaTime, player, window);*/
        }

        window.clear();

        switch (currentState) {
        case GameState::Menu:
            menu.draw(window);
            break;

        case GameState::Playing:
            background.draw(window);
            map.draw(window);;
            /*player.draw(window);
			basicEnemy.draw(window);
            rangedEnemy.draw(window);
            rangedEnemy.drawProjectiles(window);
            flyingEnemy.draw(window);*/
            entityManager.drawEntities(window);
            foreground.draw(window);
            /*chargingBoss.draw(window);*/

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

            entityManager.drawEntities(window);
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
