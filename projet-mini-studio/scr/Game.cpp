#include "../include/Game.hpp"
#include "../include/Map.hpp"
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

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            // Gérer les événements en fonction de l'état actuel du jeu
            switch (currentState) {
            case GameState::Menu:
                // Gérer les événements pour le menu
                if (event.type == Event::MouseButtonPressed) {
                    // Transition vers le jeu ou l'éditeur, selon la logique
                    currentState = GameState::Editor; // Exemple de transition vers l'éditeur
                }
                break;
            case GameState::Playing:
                // Gérer les événements pour le jeu en cours
                if (event.type == Event::MouseButtonPressed) {
                    // Logique de jeu ici
                }
                break;
            case GameState::Editor:
                // Gérer les événements pour l'éditeur de carte
                if (event.type == Event::MouseButtonPressed) {
                    int tileIndex = 1;
                    map.handleClick(event.mouseButton.x, event.mouseButton.y, tileIndex);
                }
                break;
            case GameState::Pause:
                // Gérer les événements pour la pause
                break;
            case GameState::GameOver:
                // Gérer les événements pour la fin du jeu
                break;
            }
        }

        // Gérer le rendu en fonction de l'état
        window.clear();

        switch (currentState) {
        case GameState::Menu:
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
