#include "../include/RangedEnemy.hpp"
#include <cmath>

RangedEnemy::RangedEnemy(Map& map)
    : Enemy(map), state(PATROLLING), currentWaypointIndex(0)
{
    attackCooldown = 2.0f;
    attackTimer = 0;
    detectionRadius = 300.0f; // Example radius
    fleeRadius = 100.0f;      // Example radius to start fleeing
    // Example waypoints
    waypoints.push_back(Vector2f(100, 100));
    waypoints.push_back(Vector2f(700, 100));
}

RangedEnemy::RangedEnemy(const Vector2f& size, const sf::Color& color, Map& map)
    : Enemy(size, color, map), state(PATROLLING), currentWaypointIndex(0)
{
    attackCooldown = 2.0f;
    attackTimer = 0;
    detectionRadius = 300.0f; // Example radius
    fleeRadius = 100.0f;      // Example radius to start fleeing
    // Example waypoints
    waypoints.push_back(Vector2f(100, 100));
    waypoints.push_back(Vector2f(700, 100));
}

bool RangedEnemy::isPlayerInRadius(const Vector2f& playerPosition, float radius) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = playerPosition - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    return distance <= radius;
}

void RangedEnemy::update(float dt) {
    // Call the base class update method to handle gravity
    Enemy::update(dt);
}

void RangedEnemy::update(float dt, const Player& player) {
    Vector2f playerPosition = player.getSpriteConst().getPosition(); // Get the player's position

    switch (state) {
    case PATROLLING:
        patrol(dt);
        if (isPlayerInRadius(playerPosition, detectionRadius)) {
            state = SHOOTING;
        }
        break;

    case SHOOTING:
        attackTimer += dt;
        if (attackTimer >= attackCooldown) {
            shoot();
            attackTimer = 0;
        }
        if (isPlayerInRadius(playerPosition, fleeRadius)) {
            state = FLEEING;
        }
        break;

    case FLEEING:
        flee(dt, playerPosition);
        if (!isPlayerInRadius(playerPosition, fleeRadius)) {
            if (isPlayerInRadius(playerPosition, detectionRadius)) {
                state = SHOOTING;
            }
            else {
                state = PATROLLING;
            }
        }
        break;
    }

    for (auto& projectile : projectiles) {
        projectile.move(0, 300 * dt);
    }

    Enemy::update(dt); // Call base class update to handle common enemy behavior

    if (getSprite().getPosition().y > 800)
    {
        velocity.y = 0;
    }
}

void RangedEnemy::patrol(float dt) {
    if (waypoints.empty()) return;

    Vector2f position = getSprite().getPosition();
    Vector2f target = waypoints[currentWaypointIndex];
    Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < 5.0f) { // If close to the target waypoint, move to the next
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
        target = waypoints[currentWaypointIndex];
        direction = target - position;
        length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    }

    velocity = (direction / length) * speed;
}

void RangedEnemy::flee(float dt, const Vector2f& playerPosition) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = position - playerPosition; // Move away from the player
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    velocity = (direction / length) * speed;
}

void RangedEnemy::shoot() {
    sf::RectangleShape projectile(Vector2f(10, 20));
    projectile.setFillColor(sf::Color::Red);
    projectile.setPosition(getSprite().getPosition().x, getSprite().getPosition().y - 20);

    projectiles.push_back(projectile);
}

void RangedEnemy::drawProjectiles(sf::RenderWindow& window) {
    for (const auto& projectile : projectiles) {
        window.draw(projectile);
    }
}

void RangedEnemy::setWaypoints(const std::vector<Vector2f>& newWaypoints) {
    waypoints = newWaypoints;
    currentWaypointIndex = 0; // Reset to start from the first waypoint
}