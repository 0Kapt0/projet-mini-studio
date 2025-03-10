#include "Entity.hpp"

class Player : public Entity
{
private:
	float speed;
	Vector2f velocity;
public:
	Player();
	Player(const IntRect& rect, const Color& color);
	~Player();
	void update(float dt);
};