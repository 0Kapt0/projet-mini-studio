#include "../include/EnemyFlying.hpp"
#include <cmath>

EnemyFlying::EnemyFlying()
{
	speed = 200;
	velocity = Vector2f(0, 0);
	detectionRadius = 150.0f; // Example radius
}

EnemyFlying::EnemyFlying(const Vector2f& size, const Color& color) : Entity(size, color)
{
	speed = 300;
	velocity = Vector2f(0, 0);
	detectionRadius = 150.0f; // Example radius
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

void EnemyFlying::update(float dt, const Player& player) {
	Vector2f playerPosition = player.getSpriteConst().getPosition(); // Get the player's position

	Vector2f position = getSprite().getPosition();
	FloatRect bounds = getSprite().getGlobalBounds();

	float screenWidth = 800;
	float screenHeight = 600;

	if (isPlayerInRadius(playerPosition)) {
		Vector2f direction = playerPosition - position;
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		velocity = (direction / length) * speed;
	}
	else {
		if (position.x <= 0 && velocity.x <= 0) {
			velocity.x = speed;
		}
		else if (position.x + bounds.width >= screenWidth && velocity.x >= 0) {
			velocity.x = -speed;
		}
	}

	getSprite().move(velocity * dt);
}

void EnemyFlying::drawDetectionRadius(sf::RenderWindow& window) {
	Vector2f position = getSprite().getPosition();
	sf::CircleShape detectionCircle(detectionRadius);
	detectionCircle.setPosition(position - Vector2f(detectionRadius, detectionRadius));
	detectionCircle.setFillColor(sf::Color(255, 0, 0, 100)); // Red with transparency
	window.draw(detectionCircle);
}