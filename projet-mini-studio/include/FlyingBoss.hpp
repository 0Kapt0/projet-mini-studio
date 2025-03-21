#include "Enemy.hpp"
#include "Useful.hpp"
#include "Projectile.hpp"

class FlyingBoss : public Enemy 
{
private:
	float onGroundCooldown;
	float groundCooldown;
	float groundTimer;
	float attackCooldown;
	float attackTimer;
	vector<unique_ptr<Projectile>> projectiles;
	bool goingRight = true;

	enum State 
	{ 
		IN_AIR, 
		ON_GROUND,
		FALLING, 
		FLYING
	} state; 

public:
	FlyingBoss(Map& map, Texture& texture);
	FlyingBoss(const Vector2f& size, const Color& color, Map& map);
	~FlyingBoss();
	void update(float dt, Player& player, RenderWindow& window) override;


private:
	void inAir(float dt, Player& player);
	void fly(float dt);
	void onGround(float dt);
	void fall(float dt);
	void attack(float dt, Player& player);
	void draw(RenderWindow& window) override;
	void drawProjectiles(RenderWindow& window);
};