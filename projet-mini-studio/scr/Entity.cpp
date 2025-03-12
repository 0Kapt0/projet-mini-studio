#include "../include/Entity.hpp"

Entity::Entity(Map& map) : map(map), velocity(Vector2f(0, 0))
{
	texture.loadFromFile("res/sprites/ship.png");
	sprite.setTexture(texture);
	sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

Entity::Entity(const Vector2f& size, const Color& color, Map& map) : map(map), velocity(Vector2f(0, 0))
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

float Entity::getHeight() {
    return getSprite().getLocalBounds().height * getSprite().getScale().y;
}
float Entity::getWidth() {
    return getSprite().getLocalBounds().width * getSprite().getScale().x;
}

Texture& Entity::getTexture()
{
    return texture;
}

void Entity::isColliding(/*int x, int y, */float dt)
{
    int newX = getSpriteConst().getGlobalBounds().left + velocity.x * dt;
    int newY = getSpriteConst().getGlobalBounds().top + velocity.y * dt;
    collided = false;
    // Vérifie la collision avant d'appliquer le mouvement
    if (velocity.x > 0 || dashDirection.x > 0)
    {
        if (map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>((getSprite().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width) / 32)) * 32 + 32 - getTexture().getSize().x / 2;
            getSprite().setPosition(newX - 0.1, getSpriteConst().getPosition().y);
            velocity.x = 0;
            dashDirection.x = 0;
            collided = true;
        }
    }
    else if (velocity.x < 0 || dashDirection.x < 0)
    {
        if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(getSprite().getGlobalBounds().left / 32)) * 32 + getTexture().getSize().x / 2;
            getSprite().setPosition(newX + 0.1, getSpriteConst().getPosition().y);
            velocity.x = 0;
            dashDirection.x = 0;
            collided = true;
        }
    }

    if (velocity.y > 0 || dashDirection.y > 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY + getSpriteConst().getGlobalBounds().height))
        {
            newY = (static_cast<int>((getSprite().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) / 32)) * 32 + 32 - getTexture().getSize().y / 2;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY - 0.1);
            velocity.y = 0;
            dashDirection.y = 0;
            collided = true;
        }
    }
    else if (velocity.y < 0 || dashDirection.y < 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY))
        {
            newY = (static_cast<int>(getSprite().getGlobalBounds().top / 32)) * 32 + getTexture().getSize().y / 2;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY + 0.1);
            velocity.y = 0;
            dashDirection.y = 0;
            collided = true;
        }
    }
}

const Sprite& Entity::getSpriteConst() const
{
	return sprite;
}

bool Entity::intersects(const Entity& other) const {
	return sprite.getGlobalBounds().intersects(other.sprite.getGlobalBounds());
}

void Entity::draw(RenderWindow& window)
{
	window.draw(sprite);
}

