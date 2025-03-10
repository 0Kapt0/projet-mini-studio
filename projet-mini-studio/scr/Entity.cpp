#include "../include/Entity.hpp"

Entity::Entity()
{
	texture.loadFromFile("res/sprites/ship.png");
	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

Entity::Entity(const Vector2f& size, const Color& color)
{
	texture.create(size.x, size.y);
	Image image;
	image.create(size.x, size.y, color);
	texture.update(image);
	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

Entity::~Entity()
{
}

Sprite& Entity::getSprite()
{
	return sprite;
}

Texture& Entity::getTexture()
{
	return texture;
}

const Sprite& Entity::getSpriteConst() const
{
	return sprite;
}

void Entity::draw(RenderWindow& window)
{
	window.draw(sprite);
}

