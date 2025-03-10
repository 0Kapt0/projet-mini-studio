#include "../include/Player.hpp"

Player::Player()
{
	speed = 200;
	velocity = Vector2f(0, 0);
}

Player::Player(const IntRect& rect, const Color& color) : Entity(rect, color)
{
	speed = 200;
	velocity = Vector2f(0, 0);
}

Player::~Player()
{
}

void Player::update(float dt)
{
	velocity = Vector2f(0, 0);
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x = -speed;
	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x = speed;
	if (Keyboard::isKeyPressed(Keyboard::Up))
		velocity.y = -speed;
	if (Keyboard::isKeyPressed(Keyboard::Down))
		velocity.y = speed;
	getSprite().move(velocity * dt);
}