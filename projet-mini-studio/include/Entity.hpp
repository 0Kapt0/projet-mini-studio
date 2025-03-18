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
	int hp;
	Entity();

	Entity(const Vector2f& size, const Color& color);

	Entity(Texture& tex);

	~Entity();

	Sprite& getSprite();
	float getPosX();
	float getPosY();
	void setPosX(float newPosX);
	void setPosY(float newPosY);
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

	vector<IntRect> frames;
	int currentFrame = 0;
	float elapsedTime = 0.0f;
	float frameTime = 0.1f;
	int totalFrames = 0;
	virtual void setTexture(Texture& tex, int frameWidth, int frameHeight, int totalFrames, float frameDuration);
	virtual void animate(float deltaTime);
};

#endif