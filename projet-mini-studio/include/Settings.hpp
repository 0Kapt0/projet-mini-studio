#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Settings {
public:
	Texture returnTexture;
	Sprite returnSprite;
	Texture setsoundsTexture;
	Sprite setsoundsSprite;
	Texture setmusicsTexture;
	Sprite setmusicsSprite;
	Texture setframesTexture;
	Sprite setframesSprite;
	Texture backgroundTextureSettings;
	Settings();
	~Settings();
	void draw(RenderWindow& window);
private:

};

#endif