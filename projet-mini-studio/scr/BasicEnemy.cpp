#include "../include/BasicEnemy.hpp"
#include <cmath>
#include <iostream>

BasicEnemy::BasicEnemy(Map& map)
    : Enemy(map), currentWaypointIndex(0), state(PATROLLING)
{
    speed = 200;
    velocity = Vector2f(0, 0);
    detectionRadius = 300.0f; // Example radius
    // Example waypoints
    waypoints.push_back(Vector2f(100, 100));
    waypoints.push_back(Vector2f(700, 100));
    type = "BasicEnemy";
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
    type = "BasicEnemy";
}

BasicEnemy::~BasicEnemy()
{
}

bool BasicEnemy::isPlayerInRadius(const Vector2f& playerPosition) {
    Vector2f position = getSprite().getPosition();
    Vector2f direction = playerPosition - position;
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    return distance <= detectionRadius;
}

bool BasicEnemy::isTouchingPlayer(const Player& player) {
    return getSprite().getGlobalBounds().intersects(player.getSpriteConst().getGlobalBounds());
}

void BasicEnemy::update(/*float dt, const Player& player*/float dt, Player& player, RenderWindow& window) {
    Vector2f playerPosition = player.getSpriteConst().getPosition(); // Get the player's position

    // Call the base class update method to handle gravity
    Enemy::update(dt, player, window);

    switch (state) {
    case PATROLLING:
        patrol(dt);
        if (isPlayerInRadius(playerPosition)) {
            //cout << "Transitioning to CHASING state\n";
            state = CHASING;
        }
        break;

    case CHASING:
        chase(dt, playerPosition);
        if (isTouchingPlayer(player)) {
            //cout << "Transitioning to ATTACKING state\n";
            state = ATTACKING;
        }
        else if (!isPlayerInRadius(playerPosition)) {
            //cout << "Transitioning to PATROLLING state\n";
            state = PATROLLING;
        }
        break;

    case ATTACKING:
        attack(player);
        //cout << "Transitioning to CHASING state\n";
        state = CHASING;
        break;
    }

    getSprite().move(velocity * dt);

    // Ensure the enemy is patrolling if not chasing or attacking
    if (state != CHASING && state != ATTACKING) {
        //cout << "Transitioning to PATROLLING state\n";
        state = PATROLLING;
    }
}

void BasicEnemy::patrol(float dt) {
    //cout << "Patrolling\n";
    if (waypoints.empty()) return;

    Vector2f position = getSprite().getPosition();
    Vector2f target = waypoints[currentWaypointIndex];
    Vector2f direction = target - position;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length < 5.0f) { // If close to the target waypoint, move to the next
        currentWaypointIndex = (currentWaypointIndex + 1) % waypoints.size();
        target = waypoints[currentWaypointIndex];
        direction = target - position;
        length = sqrt(direction.x * direction.x + direction.y * direction.y);
    }

    velocity = (direction / length) * speed;
}

void BasicEnemy::chase(float dt, const Vector2f& playerPosition) {
    //cout << "Chasing\n";
    Vector2f position = getSprite().getPosition();
    Vector2f direction = playerPosition - position;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    velocity = (direction / length) * speed;
}

void BasicEnemy::attack(const Player& player) {
    //cout << "Attacking\n";
    // Implement attack logic
    // For now, we'll just transition to CHASING for demonstration
    state = CHASING;
}

void BasicEnemy::drawDetectionRadius(RenderWindow& window) {
    Vector2f position = getSprite().getPosition();
    CircleShape detectionCircle(detectionRadius);
    detectionCircle.setPosition(position - Vector2f(detectionRadius, detectionRadius));
    detectionCircle.setFillColor(Color(255, 0, 0, 100)); // Red with transparency
    window.draw(detectionCircle);
}

void BasicEnemy::setWaypoints(const vector<Vector2f>& newWaypoints) {
    waypoints = newWaypoints;
    currentWaypointIndex = 0; // Reset to start from the first waypoint
}