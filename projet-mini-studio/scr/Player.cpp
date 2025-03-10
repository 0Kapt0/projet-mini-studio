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
	if (!Keyboard::isKeyPressed(Keyboard::Up) && jumpNum < 2)
	{
		canJump = true;
	}
	
	velocity.y -= 9.8f;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x = -speed;
	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x = speed;
	if (Keyboard::isKeyPressed(Keyboard::Up) && canJump)
	{
		velocity.y += speed;
		jumpNum++;
		canJump = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down))
		velocity.y -= speed;
	getSprite().move(velocity * dt);
}