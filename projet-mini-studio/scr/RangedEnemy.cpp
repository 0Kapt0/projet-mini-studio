#include "../include/RangedEnemy.hpp"

RangedEnemy::RangedEnemy(Map& map) : Enemy(map) {
    attackCooldown = 2.0f;
    attackTimer = 0;
}

RangedEnemy::RangedEnemy(const Vector2f& size, const Color& color, Map& map)
    : Enemy(size, color, map) {
    attackCooldown = 2.0f;
    attackTimer = 0;
}

void RangedEnemy::update(float dt) {
    Enemy::update(dt);

    attackTimer += dt;
    if (attackTimer >= attackCooldown) {
        shoot();
        attackTimer = 0;
    }

    for (auto& projectile : projectiles) {
        projectile.move(0, 300 * dt);
    }
}

void RangedEnemy::shoot() {
    RectangleShape projectile(Vector2f(10, 20));
    projectile.setFillColor(Color::Red);
    projectile.setPosition(getSprite().getPosition().x, getSprite().getPosition().y - 20);

    projectiles.push_back(projectile);
}

void RangedEnemy::drawProjectiles(RenderWindow& window) {
    for (const auto& projectile : projectiles) {
        window.draw(projectile);
    }
}
