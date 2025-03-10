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
    cout << "Le jeu est d�truit\n";
}

void Game::run() {
    RenderWindow window(VideoMode(1920, 1080), "Map Editor");
    window.setFramerateLimit(60);
    // D�finir l'�tat initial du jeu
    GameState currentState = GameState::Menu;

    Map map("assets/tileset/Tileset_Grass.png", "assets/map/Lobby.txt");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            // G�rer les �v�nements en fonction de l'�tat actuel du jeu
            switch (currentState) {
            case GameState::Menu:
                // G�rer les �v�nements pour le menu
                if (event.type == Event::MouseButtonPressed) {
                    // Transition vers le jeu ou l'�diteur, selon la logique
                    currentState = GameState::Editor; // Exemple de transition vers l'�diteur
                }
                break;
            case GameState::Playing:
                // G�rer les �v�nements pour le jeu en cours
                if (event.type == Event::MouseButtonPressed) {
                    // Logique de jeu ici
                }
                break;
            case GameState::Editor:
                // G�rer les �v�nements pour l'�diteur de carte
                if (event.type == Event::MouseButtonPressed) {
                    int tileIndex = 1;
                    map.handleClick(event.mouseButton.x, event.mouseButton.y, tileIndex);
                }
                break;
            case GameState::Pause:
                // G�rer les �v�nements pour la pause
                break;
            case GameState::GameOver:
                // G�rer les �v�nements pour la fin du jeu
                break;
            }
        }

        // G�rer le rendu en fonction de l'�tat
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

    // Sauvegarder l'�tat final de la carte si n�cessaire
    if (currentState == GameState::Editor) {
        map.saveMap("assets/map/Lobby.txt");
    }

}
