#include "../include/TextureManager.hpp"

TextureManager::TextureManager() {
	//loadFromFile
	checkpointTexture.loadFromFile("assets/animation/checkpoint/tv animation.png");
	playerTexture.loadFromFile("assets/animation/player/base animations.png");

}
