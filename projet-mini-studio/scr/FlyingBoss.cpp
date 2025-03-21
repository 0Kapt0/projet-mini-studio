#include "../include/FlyingBoss.hpp"

FlyingBoss::FlyingBoss(Map& map, Texture& texture) : Enemy(map, texture)
{
	this->attackCooldown = 0.3f;
	this->attackTimer = 0.0f;
	this->state = FLYING;
	this->groundCooldown = 10.0f;
	this->onGroundCooldown = 3.0f;
	this->groundTimer = 0.0f;
	hp = 100;
}

FlyingBoss::FlyingBoss(const Vector2f& size, const Color& color, Map& map) : Enemy(size, color, map)
{
	this->attackCooldown = 0.3f;
	this->attackTimer = 0.0f;
	this->state = FLYING;
	this->groundCooldown = 5.0f;
	this->onGroundCooldown = 3.0f;
	this->groundTimer = 0.0f;
	hp = 100;
}

FlyingBoss::~FlyingBoss()
{
}

void FlyingBoss::update(float dt, Player& player, RenderWindow& window)
{
	
	if (this->state == IN_AIR)
	{
		this->groundTimer += dt;
		if (this->groundTimer >= this->groundCooldown)
		{
			this->state = FALLING;
			this->groundTimer = 0.0f;
		}
		this->inAir(dt, player);
	}

	else if (this->state == FALLING)
	{

		if (map.isColliding(getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width,
			getSprite().getGlobalBounds().top + getSprite().getGlobalBounds().height + speed * dt) ||
			map.isColliding(getSprite().getGlobalBounds().left,
				getSprite().getGlobalBounds().top + getSprite().getGlobalBounds().height + speed * dt) ||
			map.isColliding(getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width / 2,
				getSprite().getGlobalBounds().top + getSprite().getGlobalBounds().height + speed * dt))
		{
			this->state = ON_GROUND;
			this->groundTimer = 0.0f;
		}
		else
		{
			this->fall(dt);
		}
	}

	else if (this->state == ON_GROUND)
	{
		this->groundTimer += dt;

		if (this->groundTimer >= this->onGroundCooldown)
		{
			this->state = FLYING;
			this->groundTimer = 0.0f;
		}
		else
		{
			this->onGround(dt);
		}
	}

	else if (this->state == FLYING)
	{
		if (getSprite().getPosition().y < 500)
		{
			this->state = IN_AIR;
			this->groundTimer = 0.0f;
		}
		else
		{
			this->fly(dt);
		}
	}

	for ( auto& projectile : projectiles)
	{
		projectile->move(dt);
		if (projectile->getSprite().getGlobalBounds().intersects(player.getSprite().getGlobalBounds()))
		{
			player.hp--;
			projectile->toBeDeleted = true;
		}
	}

	projectiles.erase(remove_if(projectiles.begin(), projectiles.end(),
		[](const unique_ptr<Projectile>& proj) { return proj->toBeDeleted; }),
		projectiles.end());

	getSprite().move(velocity * dt);
}

void FlyingBoss::inAir(float dt, Player& player)
{
	this->attackTimer += dt;

	if (this->attackTimer >= this->attackCooldown)
	{
		this->attack(dt, player);
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
	velocity.y = 0;
	velocity.x = 0;
}

void FlyingBoss::fall(float dt)
{
	velocity.y += 20;
	velocity.x = 0;
}

void FlyingBoss::attack(float dt, Player& player)
{
	Vector2f direction = Vector2f(randomNumber(player.getSpriteConst().getPosition().x - getSpriteConst().getPosition().x - 150, player.getSpriteConst().getPosition().x - getSpriteConst().getPosition().x + 150),
		randomNumber(player.getSpriteConst().getPosition().y - getSpriteConst().getPosition().y - 150, player.getSpriteConst().getPosition().y - getSpriteConst().getPosition().y + 150));
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction /= length;
	
	projectiles.push_back(make_unique<Projectile>(getSprite().getPosition(), direction, Color( 255, 0, 0, 255 ), map));
}

void FlyingBoss::draw(RenderWindow& window)
{
	window.draw(getSprite());
	drawProjectiles(window);
}

void FlyingBoss::drawProjectiles(RenderWindow& window)
{
	for (const auto& projectile : projectiles) 
	{
		projectile->draw(window);
	}
}