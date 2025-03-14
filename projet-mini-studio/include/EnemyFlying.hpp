#ifndef ENEMYFLYING_HPP
#define ENEMYFLYING_HPP

#include "Enemy.hpp"
#include "Player.hpp"
#include <vector>

class EnemyFlying : public Enemy {
protected:
    float detectionRadius;
    std::vector<Vector2f> waypoints;
    size_t currentWaypointIndex;
    Vector2f lastKnownPlayerPosition;

    enum State {
        PATROLLING,
        CHASING,
        SEARCHING,
        ATTACKING
    } state;

public:
    EnemyFlying(Map& map);
    EnemyFlying(const Vector2f& size, const Color& color, Map& map);
    virtual ~EnemyFlying();

    virtual void update(float dt, const Player& player);
    bool isPlayerInRadius(const Vector2f& playerPosition);
    void drawDetectionRadius(sf::RenderWindow& window);

    void setWaypoints(const std::vector<Vector2f>& newWaypoints);

private:
    void patrol(float dt);
    void chase(float dt, const Vector2f& playerPosition);
    void search(float dt);
    void attack(const Player& player);
};

#endif