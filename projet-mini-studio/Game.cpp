#include "Game.h"
#include <iostream>

using namespace sf;
using namespace std;

Game::Game() : window(VideoMode(1920, 1080), "Mini Studio 2D", Style::None) {
	window.setFramerateLimit(60);
}

Game::~Game() {
	cout << "Le jeu est détruit\n";
}

