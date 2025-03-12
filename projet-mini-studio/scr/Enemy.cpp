#include "../include/Enemy.hpp"

using namespace std;
using namespace sf;

Enemy::Enemy(Map& map) 
   : Entity(), map(map)
{
    this->map = map;
	speed = 200;
	velocity = Vector2f(0, 0);
}

Enemy::Enemy(const Vector2f& size, const Color& color, Map& map) : Entity(size, color), map(map)
{
    this->map = map;
	speed = 300;
	velocity = Vector2f(0, 0);
}

Enemy::~Enemy()
{
}

void Enemy::update(float dt) {
	velocity.y += 14.8f;
   
	Vector2f position = getSprite().getPosition();
	FloatRect bounds = getSprite().getGlobalBounds();

	float screenWidth = 800;
	float screenHeight = 600;


	if (position.x <= 0 && velocity.x <= 0) {
		velocity.x = speed; 
	}
	else if (position.x + bounds.width >= screenWidth && velocity.x >= 0) {
		velocity.x = -speed; 
	}

	if (getSprite().getPosition().y > 800)
	{
		velocity.y = 0;
	}
    isColliding(getSpriteConst().getPosition().x, getSpriteConst().getPosition().y, dt);

    getSprite().move(velocity * dt);

}

void Enemy::isColliding(int x, int y, float dt)
{
    int newX = getSpriteConst().getPosition().x + velocity.x * dt;
    int newY = getSpriteConst().getPosition().y + velocity.y * dt;

    collided = false;

    // V�rifie la collision avant d'appliquer le mouvement
    if (map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
    {
        newX = (static_cast<int>((getSprite().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width) / 32)) * 32 + 32 - getTexture().getSize().x / 2;
        getSprite().setPosition(newX - 0.1, getSpriteConst().getPosition().y);
        velocity.x = 0;
        collided = true;
    }

    else if (velocity.x < 0)
    {
        if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(getSprite().getGlobalBounds().left / 32)) * 32 + getTexture().getSize().x / 2;
            getSprite().setPosition(newX + 0.1, getSpriteConst().getPosition().y);
            velocity.x = 0;
            collided = true;
        }
    }
    if (velocity.y > 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY + getSpriteConst().getGlobalBounds().height))
        {
            newY = (static_cast<int>((getSprite().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) / 32)) * 32 + 32 - getTexture().getSize().y / 2;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY - 0.1);
            velocity.y = 0;
            collided = true;
        }
    }
    else if (velocity.y < 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY))
        {
            newY = (static_cast<int>(getSprite().getGlobalBounds().top / 32)) * 32 + getTexture().getSize().y / 2;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY + 0.1);
            velocity.y = 0;
            collided = true;
        }
    }
}