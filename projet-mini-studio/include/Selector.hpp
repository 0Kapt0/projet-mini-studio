#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Selector {
public:
	Texture level1Button;
	Sprite level1Sprite;
	Texture level2Button;
	Sprite level2Sprite;
	Texture level3Button;
	Sprite level3Sprite;
	Texture backgroundTexture;
	Selector();
	~Selector();
	void draw(RenderWindow& window);
private:

};

#endif