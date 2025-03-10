#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Menu {
public :
	Texture playButton;
	Texture editmodeButton;
	Sprite editSprite;
	Texture settingsButton;
	Texture menuButton;
	Texture backgroundTexture;
	Menu();
	~Menu();
	void draw(RenderWindow& window);
private:

};

#endif