#include "../include/Enemy.hpp"

Enemy::Enemy(Map& map) : Entity(map)
{
	speed = 200;
	velocity = Vector2f(0, 0);
}

Enemy::Enemy(const Vector2f& size, const Color& color, Map& map) : Entity(size, color, map)
{
	speed = 300;
	velocity = Vector2f(0, 0);
}

Enemy::~Enemy()
{
}

void Enemy::update(float dt) {

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

	getSprite().move(velocity * dt);
}