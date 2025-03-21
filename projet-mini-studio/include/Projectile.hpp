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
    RectangleShape shape;
    Map& map;

public:
    Projectile(const Vector2f& startPosition, const Vector2f& direction, const Color& color, Map& _map, float speed = 600.0f);
    void collision();
    void move(float dt);
    void draw(RenderWindow& window) override;
    void reverseDirection();
    bool toBeDeleted;
    bool reversed = false;
};

#endif // PROJECTILE_HPP