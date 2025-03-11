#ifndef RANGEDENEMY_HPP
#define RANGEDENEMY_HPP

#include "Enemy.hpp"
#include <vector>

class RangedEnemy : public Enemy {
private:
    float attackCooldown;
    float attackTimer;
    vector<RectangleShape> projectiles;

public:
    RangedEnemy(Map& map);
    RangedEnemy(const Vector2f& size, const Color& color, Map& map);

    void update(float dt) override;
    void shoot();
    void drawProjectiles(sf::RenderWindow& window);
};

#endif

