#ifndef RANGEDENEMY_HPP
#define RANGEDENEMY_HPP

#include "Enemy.hpp"
#include "Player.hpp"
#include <vector>

class RangedEnemy : public Enemy {
private:
    float attackCooldown;
    float attackTimer;
    std::vector<sf::RectangleShape> projectiles;
    float detectionRadius;
    float fleeRadius;
    Vector2f lastKnownPlayerPosition;
    std::vector<Vector2f> waypoints;
    size_t currentWaypointIndex;

    enum State {
        PATROLLING,
        SHOOTING,
        FLEEING
    } state;

public:
    RangedEnemy(Map& map, Texture& texture);
    RangedEnemy(const Vector2f& size, const sf::Color& color, Map& map);

    void update(float dt, Player& player, RenderWindow& window) override;
    //void update(float dt, const Player& player);
    void shoot();
    //void drawProjectiles(sf::RenderWindow& window);
    void draw(RenderWindow& window) override;

    bool isPlayerInRadius(const Vector2f& playerPosition, float radius);
    void setWaypoints(const std::vector<Vector2f>& newWaypoints);

private:
    void patrol(float dt);
    void flee(float dt, const Vector2f& playerPosition);
};

#endif