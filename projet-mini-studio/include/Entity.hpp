#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
using namespace sf;

class Entity
{
private:
	Texture texture;
	Sprite sprite;
public:
	Entity();

	Entity(const IntRect& rect, const Color& color);

	~Entity();

	Sprite& getSprite();

	void draw(RenderWindow& window);
};

#endif