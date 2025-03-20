#include "../include/EnemyFlying.hpp"
#include <cmath>

EnemyFlying::EnemyFlying(Map& map)
    : Enemy(map), currentWaypointIndex(0), state(PATROLLING)
{
    speed = 300;
    velocity = Vector2f(0, 0);
    detectionRadius = 200.0f; 
    // Example waypoints
    waypoints.push_back(Vector2f(100, 500));
    waypoints.push_back(Vector2f(700, 500));
}

EnemyFlying::EnemyFlying(const Vector2f& size, const Color& color, Map& map)
    : Enemy(size, color, map), currentWaypointIndex(0), state(PATROLLING)
{
    speed = 300;
    velocity = Vector2f(0, 0);
    detectionRadius = 200.0f; 
    // Example waypoints
    waypoints.push_back(Vector2f(100, 100));
    waypoints.push_back(Vector2f(700, 100));
}

EnemyFlying::~EnemyFlying()
{
}

bool EnemyFlying::isPlayerInRadius(const Vector2f& playerPosition) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = playerPosition - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    return distance <= detectionRadius;
}

void EnemyFlying::update(/*float dt, const Player& player*/float dt, Player& player, RenderWindow& window) {
    invincibilityAfterHit(dt);
    Vector2f playerPosition = player.getSpriteConst().getPosition();

    switch (state) {
    case PATROLLING:
        patrol(dt);
        if (isPlayerInRadius(playerPosition)) {
            state = CHASING;
        }
        break;

    case CHASING:
        chase(dt, playerPosition);
        if (!isPlayerInRadius(playerPosition)) {
            lastKnownPlayerPosition = playerPosition;
            state = SEARCHING;
        }
        break;

    case SEARCHING:
        search(dt);
        if (isPlayerInRadius(playerPosition)) {
            state = CHASING;
        }
        break;

    case ATTACKING:
        attack(player);
        break;
    }

    getSprite().move(velocity * dt);
}

void EnemyFlying::patrol(float dt) {
    if (waypoints.empty()) return;

    Vector2f position = getSprite().getPosition();
    Vector2f target = waypoints[currentWaypointIndex];
    Vector2f direction = target - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < 5.0f) {
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
        target = waypoints[currentWaypointIndex];
        direction = target - position;
        length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    }

    velocity = (direction / length) * speed;
}

void EnemyFlying::chase(float dt, const Vector2f& playerPosition) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = playerPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    velocity = (direction / length) * speed;

    if (length < 5.0f) {
        state = ATTACKING;
    }
}

void EnemyFlying::search(float dt) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = lastKnownPlayerPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    velocity = (direction / length) * speed;

    if (length < 5.0f) {
        state = PATROLLING;
    }
}

void EnemyFlying::attack(const Player& player) {
    // Implement attack logic
    // For now, we'll just reset to PATROLLING for demonstration
    state = PATROLLING;
}

void EnemyFlying::drawDetectionRadius(sf::RenderWindow& window) {
    Vector2f position = getSprite().getPosition();
    sf::CircleShape detectionCircle(detectionRadius);
    detectionCircle.setPosition(position - Vector2f(detectionRadius, detectionRadius));
    detectionCircle.setFillColor(sf::Color(255, 0, 0, 100));
    window.draw(detectionCircle);
}

void EnemyFlying::setWaypoints(const std::vector<Vector2f>& newWaypoints) {
    waypoints = newWaypoints;
    currentWaypointIndex = 0;
}