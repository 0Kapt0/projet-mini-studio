#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp"

using namespace sf;

class Projectile : public Entity 
{
private:
    Vector2f direction;
    float speed;

public:
    Projectile(const Vector2f& startPosition, const Vector2f& direction, const Color& color, float speed = 300.0f);
    void move(float dt);
    void draw(RenderWindow& window) override;
};

#endif // PROJECTILE_HPP