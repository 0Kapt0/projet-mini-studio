#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp" 

class Enemy : public Entity {
protected:
    /*float speed;
    Vector2f velocity;*/

public:
    Enemy(Map& map);
    Enemy(const Vector2f& size, const Color& color, Map& map);
    virtual ~Enemy();

    virtual void update(float dt);
};

#endif
