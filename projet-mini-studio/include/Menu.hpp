#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Menu {
public :
	Texture playButton;
	Sprite playSprite;
	Texture editmodeButton;
	Sprite editSprite;
	Texture settingsButton;
	Sprite settingSprite;
	Texture menuButton;
	Sprite menuSprite;
	Texture backgroundTexture;
	Texture ATexture;
	Texture BTexture;
	Texture YTexture;
	Texture XTexture;
	Sprite ASprite;
	Sprite BSprite;
	Sprite YSprite;
	Sprite XSprite;
	Menu();
	~Menu();
	void draw(RenderWindow& window);
private:

};

#endif