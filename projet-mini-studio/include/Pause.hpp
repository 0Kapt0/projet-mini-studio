#ifndef PAUSE_HPP
#define PAUSE_HPP

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Pause {
public:
	Texture playButtonPause;
	Sprite playSpritePause;
	Texture menuButtonPause;
	Sprite menuSpritePause;
	Texture backgroundTexture;
	Pause();
	~Pause();
	void draw(RenderWindow& window);
private:
};

#endif