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
	Texture chargingBossTexture;
	Texture eBoss2Texture;
	Texture eBoss3Texture;
	Texture rangedProjectileTexture;
	Texture boss2ProjectileTexture;
	Texture boss3ProjectileTexture;


	TextureManager();
};


#endif
