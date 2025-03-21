#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream> 
using namespace sf;

class TextureManager {
public:
	Texture playerTexture;
	Texture checkpointTexture;
	Texture heartTexture;
	Texture eBasicTexture;
	Texture eFlyingTexture;
	Texture eRangedTexture;
	Texture eBoss1Texture;
	Texture eBoss2Texture;
	Texture eBoss3Texture;
	Texture rangedProjectileTexture;
	Texture boss2ProjectileTexture;
	Texture boss3ProjectileTexture;

	Texture chargingBossTexture;

	TextureManager();
};


#endif
