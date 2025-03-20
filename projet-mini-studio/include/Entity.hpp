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
	int hp = 3;
	Entity();

	Entity(const Vector2f& size, const Color& color);

	~Entity();

	Sprite& getSprite();
	float getHeight();
	float getWidth();
	bool collided = false;
	bool invincible = false;
	float invincibilityTimer = 0;
	float invincibilityMaxTime = 1; //modif dans constructeur player ou enemy
	void invincibilityAfterHit(float dt);
	std::string type = "null";
 	bool toBeDeleted = false;
	
	Texture& getTexture();

	bool intersects(const Entity& other) const;

	const Sprite& getSpriteConst() const;

	virtual void draw(RenderWindow& window);
};

#endif