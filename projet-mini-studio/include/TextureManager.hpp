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
	Texture chargingBossTexture;

	TextureManager();
};


#endif
