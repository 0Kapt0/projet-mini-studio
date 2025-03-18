#include "../include/TextureManager.hpp"

TextureManager::TextureManager() {
	//loadFromFile
	checkpointTexture.loadFromFile("assets/animation/checkpoint/tv.png");
	playerTexture.loadFromFile("assets/animation/player/movement.png");

}
