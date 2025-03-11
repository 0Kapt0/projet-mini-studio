#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.hpp"
using namespace sf;
using namespace std;

class Entity
{
private:
	Texture texture;
	Sprite sprite;
public:

	Map& map;
	Vector2f velocity;
	float speed;
	
	Entity(Map& map);

	Entity(const Vector2f& size, const Color& color, Map& map);

	~Entity();

	Sprite& getSprite();
	
	Texture& getTexture();

	bool isColliding(/*int x, int y, */float dt);

	const Sprite& getSpriteConst() const;

	virtual void draw(RenderWindow& window);
};

#endif