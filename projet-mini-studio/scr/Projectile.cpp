#include "../include/Projectile.hpp"

Projectile::Projectile(const Vector2f& startPosition, const Vector2f& direction, const Color& color, float speed)
    : Entity(Vector2f(10, 10), Color::Red), direction(direction), speed(speed)
{
    getSprite().setPosition(startPosition);
}

void Projectile::move(float dt)
{
    getSprite().move(direction * speed * dt);
}

void Projectile::draw(RenderWindow& window)
{
    window.draw(getSprite());
}