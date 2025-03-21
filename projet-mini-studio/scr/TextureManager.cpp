#include "../include/TextureManager.hpp"

TextureManager::TextureManager() 
{
	checkpointTexture.loadFromFile("assets/animation/checkpoint/tv animation.png");
	playerTexture.loadFromFile("assets/animation/player/animation.png");
	heartTexture.loadFromFile("assets/ui/heart.png");
	eBasicTexture.loadFromFile("assets/animation/enemies/basic_enemy.png");
	eFlyingTexture.loadFromFile("assets/animation/enemies/flying_enemy.png");
	eRangedTexture.loadFromFile("assets/animation/enemies/ranged_enemy.png");
	chargingBossTexture.loadFromFile("assets/animation/charging boss.png");
	eBoss2Texture.loadFromFile("assets/animation/animation/enemies/boss2.png");
	//eBoss3Texture.loadFromFile("assets/animation/enemies/boss3.png");
	eBoss3Texture.loadFromFile("assets/animation/enemies/boss3.png");
	rangedProjectileTexture.loadFromFile("assets/animation/enemies/ranged_proj.png");
	//boss2ProjectileTexture.loadFromFile("assets/animation/enemies/boss2_proj.png");
	//boss3ProjectileTexture.loadFromFile("assets/animation/enemies/boss3_proj.png");
}