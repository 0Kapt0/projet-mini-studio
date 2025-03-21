#include "../include/Projectile.hpp"

Projectile::Projectile(const Vector2f& startPosition, const Vector2f& direction, const Color& color, Map& _map, float speed)
    : Entity(Vector2f(10, 10), color), direction(direction), speed(speed), toBeDeleted(false), map(_map)
{
    getSprite().setPosition(startPosition);
    shape.setFillColor(Color::Red);
    shape.setSize(Vector2f(5, 5));
}

void Projectile::move(float dt)
{
    getSprite().move(direction * speed * dt);
    collision();
}

void Projectile::collision()
{
    if (map.isColliding(getSprite().getGlobalBounds().left, getSprite().getGlobalBounds().top) ||
        map.isColliding(getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width, getSprite().getGlobalBounds().top) ||
        map.isColliding(getSprite().getGlobalBounds().left, getSprite().getGlobalBounds().top + getSprite().getGlobalBounds().height) ||
        map.isColliding(getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width, getSprite().getGlobalBounds().top + getSprite().getGlobalBounds().height))
    {
        toBeDeleted = true;
    }
}

void Projectile::draw(RenderWindow& window)
{
    cout << "drawing proj at : " << getSprite().getPosition().x << "     " << getSprite().getPosition().y << endl;
    shape.setPosition(getSprite().getPosition());
    window.draw(getSprite());
}

void Projectile::reverseDirection() 
{
    if (!reversed)
    {
        direction.x *= -1;
        direction.y *= -1;
        reversed = true;
    }
}