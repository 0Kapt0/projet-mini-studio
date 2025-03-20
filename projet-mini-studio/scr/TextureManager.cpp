#include "../include/TextureManager.hpp"

TextureManager::TextureManager() {
	checkpointTexture.loadFromFile("assets/animation/checkpoint/tv animation.png");
	playerTexture.loadFromFile("assets/animation/player/base animations.png");
	heartTexture.loadFromFile("assets/ui/heart.png");
	chargingBossTexture.loadFromFile("assets/animation/charging boss/charging boss.png");

}
