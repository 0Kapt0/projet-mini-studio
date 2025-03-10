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
	Player(const IntRect& rect, const Color& color);
	~Player();
	void update(float dt);
};