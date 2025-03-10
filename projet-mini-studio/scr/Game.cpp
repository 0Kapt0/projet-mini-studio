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
    cout << "Le jeu est d�truit\n";
}

void Game::run() {
    RenderWindow window(VideoMode(1920, 1080), "Map Editor");
    window.setFramerateLimit(60);
    // D�finir l'�tat initial du jeu
    GameState currentState = GameState::Menu;
    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");
    Menu menu;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            // G�rer les �v�nements en fonction de l'�tat actuel du jeu
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
        // G�rer le rendu en fonction de l'�tat
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
    // Sauvegarder l'�tat final de la carte si n�cessaire
    if (currentState == GameState::Editor) {
        map.saveMap("assets/map/Lobby.txt");
    }
}
