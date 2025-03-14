#include "../include/FlyingBoss.hpp"

FlyingBoss::FlyingBoss(Map& map) : Enemy(map)
{
	this->attackCooldown = 1.0f;
	this->attackTimer = 0.0f;
	this->state = FLYING;
	this->groundCooldown = 10.0f;
	this->onGroundCooldown = 3.0f;
	this->groundTimer = 0.0f;
	hp = 100;
}

FlyingBoss::FlyingBoss(const Vector2f& size, const Color& color, Map& map) : Enemy(size, color, map)
{
	this->attackCooldown = 1.0f;
	this->attackTimer = 0.0f;
	this->state = FLYING;
	this->groundCooldown = 10.0f;
	this->onGroundCooldown = 3.0f;
	this->groundTimer = 0.0f;
	hp = 100;
}

FlyingBoss::~FlyingBoss()
{
}

void FlyingBoss::update(float dt)
{
	//Enemy::update(dt);
	this->attackTimer += dt;
	this->groundTimer += dt;
	if (this->state == IN_AIR)
	{
		if (this->groundTimer >= this->groundCooldown)
		{
			this->state = FALLING;
			this->groundTimer = 0.0f;
		}
		this->fly(dt);
	}
	if (this->state == FALLING)
	{
		if (map.isColliding(getSprite().getPosition().x, getSprite().getPosition().y + speed * dt))
		{
			this->state = ON_GROUND;
		}
		this->fall(dt);
	}
	else if (this->state == ON_GROUND)
	{
		if (this->groundTimer >= this->onGroundCooldown)
		{
			this->state = FLYING;
			this->groundTimer = 0.0f;
		}
		this->onGround(dt);
	}
	else if (this->state == FLYING)
	{
		if (getSprite().getPosition().y < 200)
		{
			this->state = IN_AIR;
			this->groundTimer = 0.0f;
		}
		this->fly(dt);
	}

	getSprite().move(velocity * dt);
}

void FlyingBoss::inAir(float dt)
{
	if (this->attackTimer >= this->attackCooldown)
	{
		this->attack();
		this->attackTimer = 0.0f;
	}

	if (goingRight)
	{
		if (getSprite().getPosition().x > 1000)
		{
			goingRight = false;
		}
		
		velocity.x = speed;
	}
	else
	{
		if (getSprite().getPosition().x < 100)
		{
			goingRight = true;
		}
		velocity.x = -speed;
	}
	velocity.y = 0;
}

void FlyingBoss::fly(float dt)
{
	velocity.y = -speed;
	velocity.x = 0;
}

void FlyingBoss::onGround(float dt)
{

}

void FlyingBoss::fall(float dt)
{
	velocity.y += 20;
	velocity.x = 0;
}

void FlyingBoss::attack()
{
	sf::RectangleShape projectile(Vector2f(10, 20));
	projectile.setFillColor(sf::Color::Red);
	projectile.setPosition(getSprite().getPosition().x, getSprite().getPosition().y - 20);

	projectiles.push_back(projectile);
}

void FlyingBoss::drawProjectiles(RenderWindow& window)
{
	for (const auto& projectile : projectiles) {
		window.draw(projectile);
	}
}