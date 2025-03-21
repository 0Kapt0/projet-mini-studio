#include "../include/FinalBoss.hpp"

FinalBoss::FinalBoss(Map& map, Texture& texture) : Enemy(map, texture) {}

FinalBoss::FinalBoss(const Vector2f& size, const Color& color, Map& map) : Enemy(size, color, map) {}

FinalBoss::~FinalBoss() {}

void FinalBoss::update(float dt, Player& player, RenderWindow& window)
{
	invincibilityAfterHit(dt);
	attackTimer += dt;
	if (attackTimer >= attackCooldown)
	{
		attack(dt, player);
		attackTimer = 0;
	}

	for (auto& projectile : projectiles)
	{
		projectile->move(dt);
		if (projectile->getSprite().getGlobalBounds().intersects(player.getSprite().getGlobalBounds()))
		{
			player.hp--;
			projectile->toBeDeleted = true;
		}

		if (projectile->getSprite().getGlobalBounds().intersects(player.attackSprite.getGlobalBounds()) && player.isAttacking())
		{
			projectile->reverseDirection();
		}
		if (projectile->getSprite().getGlobalBounds().intersects(getSprite().getGlobalBounds()) && projectile->reversed)
		{
			hp--;
		}
	}

	projectiles.erase(remove_if(projectiles.begin(), projectiles.end(),
		[](const unique_ptr<Projectile>& proj) { return proj->toBeDeleted; }),
		projectiles.end());
}



void FinalBoss::attack(float dt, Player& player) 
{
	Vector2f direction = Vector2f(player.getSpriteConst().getPosition().x - getSpriteConst().getPosition().x,
		player.getSpriteConst().getPosition().y - getSpriteConst().getPosition().y);
	float length = sqrt(direction.x * direction.x + direction.y * direction.y);
	direction /= length;

	projectiles.push_back(make_unique<Projectile>(getSprite().getPosition(), direction, Color(255, 0, 0, 255), map));
}



void FinalBoss::draw(RenderWindow& window) {
	window.draw(getSprite());
	drawProjectiles(window);
}

void FinalBoss::drawProjectiles(RenderWindow& window) {
	for (const auto& projectile : projectiles) {
		projectile->draw(window);
	}
}

