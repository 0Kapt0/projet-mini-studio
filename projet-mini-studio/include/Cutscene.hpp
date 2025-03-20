
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

private:

};

#endif