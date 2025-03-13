#include "../include/BasicEnemy.hpp"
#include <cmath>

BasicEnemy::BasicEnemy(Map& map)
    : Enemy(map), currentWaypointIndex(0), state(PATROLLING)
{
    speed = 200;
    velocity = Vector2f(0, 0);
    detectionRadius = 300.0f; // Example radius
    // Example waypoints
    waypoints.push_back(Vector2f(100, 100));
    waypoints.push_back(Vector2f(700, 100));
}

BasicEnemy::BasicEnemy(const Vector2f& size, const Color& color, Map& map)
    : Enemy(size, color, map), currentWaypointIndex(0), state(PATROLLING)
{
    speed = 300;
    velocity = Vector2f(0, 0);
    detectionRadius = 300.0f; // Example radius
    // Example waypoints
    waypoints.push_back(Vector2f(100, 100));
    waypoints.push_back(Vector2f(700, 100));
}

BasicEnemy::~BasicEnemy()
{
}

bool BasicEnemy::isPlayerInRadius(const Vector2f& playerPosition) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = playerPosition - position;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    return distance <= detectionRadius;
}

void BasicEnemy::update(float dt, const Player& player) {
    Vector2f playerPosition = player.getSpriteConst().getPosition(); // Get the player's position

    // Call the base class update method to handle gravity
    Enemy::update(dt);

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

void BasicEnemy::patrol(float dt) {
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

void BasicEnemy::chase(float dt, const Vector2f& playerPosition) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = playerPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    velocity = (direction / length) * speed;

    if (length < 5.0f) {
        state = ATTACKING;
    }
}

void BasicEnemy::search(float dt) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = lastKnownPlayerPosition - position;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    velocity = (direction / length) * speed;

    if (length < 5.0f) {
        state = PATROLLING;
    }
}

void BasicEnemy::attack(const Player& player) {
    // Implement attack logic
    // For now, we'll just reset to PATROLLING for demonstration
    state = PATROLLING;
}

void BasicEnemy::drawDetectionRadius(sf::RenderWindow& window) {
    Vector2f position = getSprite().getPosition();
    sf::CircleShape detectionCircle(detectionRadius);
    detectionCircle.setPosition(position - Vector2f(detectionRadius, detectionRadius));
    detectionCircle.setFillColor(sf::Color(255, 0, 0, 100)); // Red with transparency
    window.draw(detectionCircle);
}

void BasicEnemy::setWaypoints(const std::vector<Vector2f>& newWaypoints) {
    waypoints = newWaypoints;
    currentWaypointIndex = 0; // Reset to start from the first waypoint
}