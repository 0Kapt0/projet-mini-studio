#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream> 
using namespace sf;

class TextureManager {
public:
	TextureManager();

	Texture getPlayerTexture();
	Texture getEnemyTexture();
	Texture getBossTexture();

};


#endif
