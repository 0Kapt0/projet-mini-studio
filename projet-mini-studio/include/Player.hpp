#include "Entity.hpp"

class Player : public Entity
{
private:
	float speed;
	Vector2f velocity;
	bool canJump = true;
	int jumpNum = 0;
	bool canDash = true;
	bool dashing = false;
	Vector2f dashDirection;
	char lastInputDirection = 'N';
	float dashDuration = 0;
	float dashCooldown = 0.8;
	float dashTimer = 0;
public:
	Player();
	Player(const Vector2f& size, const Color& color);
	~Player();
	void update(float dt);
};