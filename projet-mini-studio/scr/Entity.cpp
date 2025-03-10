#include "../include/Entity.hpp"

Entity::Entity()
{
	texture.loadFromFile("res/sprites/ship.png");
	sprite.setTexture(texture);
}

Entity::Entity(const IntRect& rect, const Color& color)
{
	texture.create(rect.width, rect.height);
	Image image;
	image.create(rect.width, rect.height, color);
	texture.update(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(rect);
}

Entity::~Entity()
{
}

Sprite& Entity::getSprite()
{
	return sprite;
}

void Entity::draw(RenderWindow& window)
{
	window.draw(sprite);
}

