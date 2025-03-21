#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Selector {
public:
	int levelunlocked;
	Texture level1Button;
	Sprite level1Sprite;
	Texture level2Button;
	Sprite level2Sprite;
	Texture level2lockButton;
	Sprite level2lockSprite;
	Texture level3lockButton;
	Sprite level3lockSprite;
	Texture level3Button;
	Sprite level3Sprite;
	Texture returnTexture;
	Sprite returnSprite;
	Texture backgroundTexture;
	Texture ATexture;
	Texture BTexture;
	Texture YTexture;
	Texture XTexture;
	Sprite ASprite;
	Sprite BSprite;
	Sprite YSprite;
	Sprite XSprite;
	Selector();
	~Selector();
	void draw(RenderWindow& window);
private:
};

#endif