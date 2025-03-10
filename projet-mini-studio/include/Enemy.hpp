#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp" 

class Enemy : public Entity {
protected:
    float speed;
    Vector2f velocity;

public:
    Enemy();
    Enemy(const Vector2f& size, const Color& color);
    virtual ~Enemy();

    virtual void update(float dt);
};

#endif
