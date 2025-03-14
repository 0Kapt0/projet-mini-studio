#include "Enemy.hpp"

class FlyingBoss : public Enemy 
{
private:
	float onGroundCooldown;
	float groundCooldown;
	float groundTimer;
	float attackCooldown;
	float attackTimer;
	vector<RectangleShape> projectiles;
	int hp;
	bool goingRight = true;

	enum State 
	{ 
		IN_AIR, 
		ON_GROUND,
		FALLING, 
		FLYING
	} state; 

public:
	FlyingBoss(Map& map);
	FlyingBoss(const Vector2f& size, const Color& color, Map& map);
	~FlyingBoss();
	void update(float dt) override;

private:
	void inAir(float dt);
	void fly(float dt);
	void onGround(float dt);
	void fall(float dt);
	void attack();
	void drawProjectiles(RenderWindow& window);
};