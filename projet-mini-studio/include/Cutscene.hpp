
#ifndef CUTSCENE_HPP
#define CUTSCENE_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Cutscene {
public:
	Clock cooldownframe;
	Cutscene();
	~Cutscene();
	int currentFrame2 = 4;
	Texture cutscene2Texture[133];
	Sprite cutscene2Sprite;
	void draw(RenderWindow& window);
	Texture cutscene1Texture[89];
	Sprite cutscene1Sprite;
	void draw1(RenderWindow& window);
	int currentFrame1 = 0;
	Texture cutscene3Texture[84];
	Sprite cutscene3Sprite;
	void draw2(RenderWindow& window);
	int currentFrame3 = 0;
	Texture cutscene4Texture[84];
	Sprite cutscene4Sprite;
	void draw3(RenderWindow& window);
	int currentFrame4 = 0;

private:

};

#endif