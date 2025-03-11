#ifndef ENEMYFLYING_HPP
#define ENEMYFLYING_HPP

#include "Entity.hpp" 

class EnemyFlying : public Entity {
protected:
    float speed;
    Vector2f velocity;

public:
    EnemyFlying();
    EnemyFlying(const Vector2f& size, const Color& color);
    virtual ~EnemyFlying();

    virtual void update(float dt);
};

#endif
