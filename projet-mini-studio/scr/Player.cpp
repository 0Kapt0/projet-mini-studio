#include "../include/Player.hpp"

Player::Player()
{
	speed = 200;
	velocity = Vector2f(0, 0);
}

Player::Player(const Vector2f& size, const Color& color) : Entity(size, color)
{
	speed = 450;
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

	velocity.y += 14.8f;

	if (getSprite().getPosition().y > 200)
	{
		canJump = true;
		jumpNum = 0;
		velocity.y = 0;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x = 0;
	
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x = -speed;
	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x = speed;
	if (Keyboard::isKeyPressed(Keyboard::Up) && canJump)
	{
		velocity.y = -speed;
		jumpNum++;
		canJump = false;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) && getSprite().getPosition().y < 200)
		velocity.y += speed;
	getSprite().move(velocity * dt);
}