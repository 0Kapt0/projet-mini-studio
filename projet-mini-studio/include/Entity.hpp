#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;
using namespace std;

class Entity
{
private:
	Texture texture;
	Sprite sprite;
public:
	Entity();

	Entity(const Vector2f& size, const Color& color);

	~Entity();

	Sprite& getSprite();

	Texture& getTexture();

	bool intersects(const Entity& other) const;

	const Sprite& getSpriteConst() const;

	virtual void draw(RenderWindow& window);
};

#endif