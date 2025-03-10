#include "../include/Game.hpp"
#include "../include/Map.hpp"
#include "../include/Menu.hpp"
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

Game::Game(){
    
}

Game::~Game() {
    cout << "Le jeu est détruit\n";
}

void Game::run() {
    RenderWindow window(VideoMode(1920, 1080), "Map Editor");
    window.setFramerateLimit(60);
    // Définir l'état initial du jeu
    GameState currentState = GameState::Menu;
    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");
    Menu menu;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            // Gérer les événements en fonction de l'état actuel du jeu
            switch (currentState) {
            case GameState::Menu:
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (menu.editSprite.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
                        currentState = GameState::Editor;
                    }
                   
                }
                break;
            case GameState::Playing:
                if (event.type == Event::MouseButtonPressed) {
                }
                break;
            case GameState::Editor:
                if (event.type == Event::MouseButtonPressed) {
                    int tileIndex = 1;
                    map.handleClick(event.mouseButton.x, event.mouseButton.y, tileIndex);
                }
                break;
            case GameState::Pause:
                break;
            case GameState::GameOver:
                break;
            }
        }
        // Gérer le rendu en fonction de l'état
        window.clear();

        switch (currentState) {
        case GameState::Menu:
            menu.draw(window);
            break;
        case GameState::Playing:
            break;
        case GameState::Editor:
            map.draw(window);
            break;
        case GameState::Pause:
            break;
        case GameState::GameOver:
            break;
        }

        window.display();
    }
    // Sauvegarder l'état final de la carte si nécessaire
    if (currentState == GameState::Editor) {
        map.saveMap("assets/map/Lobby.txt");
    }
}
