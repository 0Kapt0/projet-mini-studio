#include "Entity.hpp"

class Enemy : public Entity
{
private:
	float speed;
	Vector2f velocity;

public:
	Enemy();
	Enemy(const Vector2f& size, const Color& color);
	~Enemy();
	void update(float dt);
};