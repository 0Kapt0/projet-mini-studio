#include "../include/Game.hpp"
#include "../include/Settings.hpp"
#include "../include/Map.hpp"
#include "../include/Cutscene.hpp"
#include "../include/Menu.hpp"
#include "../include/Pause.hpp"
#include "../include/Selector.hpp"
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
#include <string>
#include <array>

using namespace sf;
using namespace std;

struct LevelAssets {
    array<string, 6> backgroundTextures;
    array<string, 2> foregroundTextures;
    string mapTileset;
    string tileSelectorTileset;
    array<float, 6> backgroundSpeeds;
};

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
        "assets/tileset/tileset_green_vFinal.png",
        {1.89f, 1.89f, 1.89f, 1.89f, 1.89f, 1.89f}
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
        "assets/tileset/tilesetTownV1.png",
        {0.02f, 0.04f, 0.08f, 0.15f, 0.25f, 0.35f}
    }},
    {
    "assets/map/Level3.txt", {
      { "assets/background/BG1.png",
        "assets/background/BG2.png",
        "assets/background/BG3.png",
        "assets/background/BG4.png",
        "assets/background/BG5.png",
        "assets/background/BG6.png" },
      { "assets/foreground/foreground_cityyy.png",
        "assets/foreground/level2_foregrounyyd2.png" },
      "assets/tileset/tilesetTownV1.png",
      "assets/tileset/tilesetTownV1.png",
      {0.02f, 0.04f, 0.08f, 0.15f, 0.25f, 0.35f}
  }
}
};

void setLevel(const string& levelFile,
    Background& background,
    Foreground& foreground,
    Map& map,
    TileSelector& tileSelector)
{
    if (levelAssetsMap.find(levelFile) != levelAssetsMap.end()) {
        LevelAssets assets = levelAssetsMap[levelFile];

        if (!background.loadTextures(assets.backgroundTextures[0],
            assets.backgroundTextures[1],
            assets.backgroundTextures[2],
            assets.backgroundTextures[3],
            assets.backgroundTextures[4],
            assets.backgroundTextures[5]))
        {
            std::cerr << "Erreur de chargement du background pour " << levelFile << endl;
        }

        background.setSpeeds(assets.backgroundSpeeds);

        if (!foreground.loadTextures(assets.foregroundTextures[0],
            assets.foregroundTextures[1])) {
            std::cerr << "Erreur de chargement du foreground pour " << levelFile << endl;
        }

        map.setTileset(assets.mapTileset);
        map.generateTiles();
        tileSelector.setTileset(assets.tileSelectorTileset);

        map.loadMap(levelFile);
        map.generateTiles();

        std::cout << "Niveau " << levelFile << " charg� avec succ�s." << endl;
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
    Selector,
    Cutscene,
    GameOver
};

Game::Game() : deltaTime(0.0f) {}

Game::~Game() {
    cout << "Le jeu est d�truit\n";
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
    Cutscene cutscene;
    GameState currentState = GameState::Menu;
    Menu menu;
    Selector selector;
    Settings settings;
    Pause pause;
	SoundManager& soundManager = SoundManager::getInstance();

    // Instanciation des sons
	soundManager.loadSound("Level1Music", "assets/sfx/Level1Music.mp3");
    soundManager.loadSound("MenuMusic", "assets/sfx/menumusic.mp3");
    soundManager.loadSound("cutscene2", "assets/sfx/cutscene2.mp3");
    soundManager.loadSound("cutscene1", "assets/sfx/cutscene1.mp3");
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
    soundManager.playSound("MenuMusic");
    soundManager.setLoop("MenuMusic",true);
    while (window.isOpen()) {
        window.clear();
        Event event;
		deltaTime = clock.restart().asSeconds();
        bool isLeftMousePressed = false;
        bool isRightMousePressed = false;
        float cutscene2CooldownTime = 10.75f;
        float cutsceneCooldownTime = 7.0f;
        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed)
                window.close();

            //Gestion des �v�nements en fonction de l'�tat actuel
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
                        currentState = GameState::Selector;
                    }
                }
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (menu.settingSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Settings;
                    }
                }
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (menu.menuSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        window.close();
                    }
                }
                if (Joystick::isConnected(0))
                {
                    if (Joystick::isButtonPressed(0, 0))
                    {
                        currentState = GameState::Selector;
                    }
                    if (Joystick::isButtonPressed(0, 1))
                    {
                        window.close();
                    }
                    if (Joystick::isButtonPressed(0, 2))
                    {
                        currentState = GameState::Editor;
                    }
                    if (Joystick::isButtonPressed(0, 3))
                    {
                        currentState = GameState::Settings;
                    }
                }
                break;

            case GameState::Playing:
                if (!enemiesGenerated) {
                    entityManager.generateEnemies(map);
                    entityManager.createEntity("FinalBoss", Vector2f(1200, 1700), Vector2f(150, 150), Color::Green, map);
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
            case GameState::Selector:
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (selector.level1Sprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                         cutsceneCooldown.restart();
                         levelselected = 1;
                        currentState = GameState::Cutscene;
                        soundManager.stopSound("MenuMusic");
                        soundManager.playSound("cutscene1");
                    }
                }
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (selector.level2Sprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        if (selector.levelunlocked >= 2) {
                            cutscene2Cooldown.restart();
                            levelselected = 2;
                            currentState = GameState::Cutscene;
                            soundManager.stopSound("MenuMusic");
                            soundManager.playSound("cutscene2");
                        }
                    }
                }
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (selector.level3Sprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        if (selector.levelunlocked >= 3) {
                            cutsceneCooldown.restart();
                            levelselected = 3;
                            currentState = GameState::Cutscene;
                            soundManager.stopSound("MenuMusic");
                            soundManager.playSound("cutscene1");
                        }
                    }
                }
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (selector.returnSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Menu;
                    }
                }
                if (Joystick::isConnected(0))
                {
                    if (Joystick::isButtonPressed(0, 0))
                    {
                        currentState = GameState::Cutscene;
                    }
                    if (Joystick::isButtonPressed(0, 1))
                    {
                        currentState = GameState::Menu;
                    }
                    //if (Joystick::isButtonPressed(0, 2))
                    //{
                    //    currentState = GameState::Editor;
                    //}
                    //if (Joystick::isButtonPressed(0, 3))
                    //{
                    //    currentState = GameState::Settings;
                    //}
                }
                break;
            case GameState::Cutscene:
               
                break;

            case GameState::GameOver:
                break;
            }
        }

        if (currentState == GameState::Playing) {
            entityManager.updateEntities(event, deltaTime, window);
            entityManager.destroyEntity();
            float cameraX = entityManager.player->getSprite().getPosition().x;
            float playerY = entityManager.player->getSprite().getPosition().y;
            background.update(cameraX, playerY, true);
            foreground.update(cameraX);
        }

        if (currentState == GameState::Cutscene) {
            if (levelselected == 1) {
                if (cutsceneCooldown.getElapsedTime().asSeconds() >= cutsceneCooldownTime) {
                    currentState = GameState::Playing;
                    soundManager.playSound("Level1Music");
                }
            }
            if (levelselected == 2) {
                if (cutscene2Cooldown.getElapsedTime().asSeconds() >= cutscene2CooldownTime) {
                    currentState = GameState::Playing;
                    soundManager.playSound("Level2Music");
                }
            }
            if (levelselected == 3) {
                if (cutsceneCooldown.getElapsedTime().asSeconds() >= cutscene2CooldownTime) {
                    currentState = GameState::Playing;
                    soundManager.playSound("Level3Music");
                }
            }
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
            entityManager.player->drawHearts(window);
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

        case GameState::Selector:
            selector.draw(window);
            break;
        case GameState::Cutscene:
            if (levelselected == 1) {
                cutscene.draw1(window);
            }
            if (levelselected == 2) {
                cutscene.draw(window);
            }
            if (levelselected == 3) {
                cutscene.draw3(window);
            }
            break;
        case GameState::GameOver:
            break;
        }

        window.display();
    }

    // Sauvegarde de la carte en quittant l'�diteur
    if (currentState == GameState::Editor || currentState == GameState::Menu || (currentState == GameState::Playing)) {
        map.saveMap(currentLevelFile);
    }
}
