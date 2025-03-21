#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Gameover {
public:
	Texture backgroundTexture;
	Sprite backgroundSprite;
	Texture menuTexture;
	Sprite menuSprite;
	Texture overTexture;
	Sprite overSprite;
	Gameover();
	~Gameover();
	void draw(RenderWindow& window);
private:

};

#endif