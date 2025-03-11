#include "../include/EnemyFlying.hpp"

EnemyFlying::EnemyFlying()
{
	speed = 200;
	velocity = Vector2f(0, 0);
}

EnemyFlying::EnemyFlying(const Vector2f& size, const Color& color) : Entity(size, color)
{
	speed = 300;
	velocity = Vector2f(0, 0);
}

EnemyFlying::~EnemyFlying()
{
}

void EnemyFlying::update(float dt) {

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