#ifndef BASICENEMY_HPP
#define BASICENEMY_HPP

#include "Enemy.hpp"
#include "Player.hpp"
#include <vector>

class BasicEnemy : public Enemy {
protected:
    float speed;
    Vector2f velocity;
    float detectionRadius;
    std::vector<Vector2f> waypoints;
    size_t currentWaypointIndex;

    enum State {
        PATROLLING,
        CHASING,
        ATTACKING
    } state;

public:
    BasicEnemy(Map& map, Texture& texture);
    BasicEnemy(const Vector2f& size, const Color& color, Map& map);
    virtual ~BasicEnemy();

    //virtual void update(float dt, const Player& player);
    void update(float dt, Player& player, RenderWindow& window) override;
    bool isPlayerInRadius(const Vector2f& playerPosition);
    bool isTouchingPlayer(const Player& player);
    void drawDetectionRadius(sf::RenderWindow& window);

    void setWaypoints(const std::vector<Vector2f>& newWaypoints);
    void setPosition(const Vector2f& position);

private:
    void patrol(float dt);
    void chase(float dt, const Vector2f& playerPosition);
    void attack(const Player& player);
};

#endif