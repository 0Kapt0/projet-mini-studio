#include "Entity.hpp"

class Player : public Entity
{
private:
	float speed;
	Vector2f velocity;
	bool canJump = true;
	int jumpNum = 0;
public:
	Player();
	Player(const Vector2f& size, const Color& color);
	~Player();
	void update(float dt);
};