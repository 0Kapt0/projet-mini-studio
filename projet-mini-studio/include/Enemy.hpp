#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp" 
#include "Map.hpp"

class Enemy : public Entity {
protected:
    float speed;
    Vector2f velocity;
    Map map;

public:
    Enemy(Map& map);
    Enemy(const Vector2f& size, const Color& color, Map& map);
    virtual ~Enemy();
    virtual void update(float dt);
    virtual void isColliding(int x, int y, float dt);
};

#endif
