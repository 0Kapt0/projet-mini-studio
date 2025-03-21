#ifndef FINALBOSS_HPP
#define FINALBOSS_HPP

#include "Enemy.hpp"
#include "Projectile.hpp"

class FinalBoss :
    public Enemy
{
public:
	FinalBoss(Map& map);
	FinalBoss(const Vector2f& size, const Color& color, Map& map);
	~FinalBoss();

	void update(float dt, Player& player, RenderWindow& window) override;

private:
	float attackTimer = 0;
	float attackCooldown = 2;
	vector<unique_ptr<Projectile>> projectiles;

	void attack(float dt, Player& player);
	void draw(RenderWindow& window) override;
	void drawProjectiles(RenderWindow& window);
};



#endif