#include "../include/Player.hpp"

Player::Player() : grapple(500.0f) // Exemple de portée maximale du grappin
{
    speed = 200;
    velocity = Vector2f(0, 0);
}

Player::Player(const Vector2f& size, const Color& color) : Entity(size, color), grapple(500.0f) // Exemple de portée maximale du grappin
{
    speed = 450;
    velocity = Vector2f(0, 0);
}

Player::~Player()
{
}

void Player::update(float dt, vector<vector<int>>& map)
{
	if (!dashing) {
		dashTimer += dt;
		if (dashTimer >= dashCooldown && getSprite().getPosition().y > 200) canDash = true;

		if (!Keyboard::isKeyPressed(Keyboard::Z) && jumpNum < 2)
		{
			canJump = true;
		}

		velocity.y += 14.8f;

		if (getSprite().getPosition().y > 800)
		{
			canJump = true;
			jumpNum = 0;
			velocity.y = 0;
		}
		if (!Keyboard::isKeyPressed(Keyboard::Q) && !Keyboard::isKeyPressed(Keyboard::D))
			velocity.x = 0;

		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			velocity.x = -speed;
			lastInputDirection = 'L'; 
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			velocity.x = speed;
			lastInputDirection = 'R';
		}
		if (Keyboard::isKeyPressed(Keyboard::Z) && canJump)
		{
			velocity.y = -speed;
			jumpNum++;
			canJump = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::S) && getSprite().getPosition().y < 200)
			velocity.y += speed;
		getSprite().move(velocity * dt);

		if (Keyboard::isKeyPressed(Keyboard::Space) && canDash) {
			dashing = true;
			canDash = false;
			dashTimer = 0;
			velocity.y = 0;
			
			switch (lastInputDirection) {
				case('L'): dashDirection.x = -speed * 3.f; break;
				case('R'): dashDirection.x = speed * 3.f; break;
			}
			//dashDirection = velocity * 3.f; // si on veut un dash dans toutes les directions
		}
	}

	else { //(dashing)
		dashDuration += dt;
		if (dashDuration >= 0.2) {
			dashing = false;
			dashDuration = 0;
		}
		getSprite().move(dashDirection.x * dt, 0);
	}
	if (grapple.isActive()) {
		grapple.updateStartPosition(getSprite().getPosition());
	}
	// Mettre à jour le grappin
	grapple.update(dt);
}

void Player::handleInput(const Event& event, const RenderWindow& window)
{
	if (event.type == Event::KeyPressed && event.key.code == Keyboard::A)
	{
		// Lancer le grappin depuis la position actuelle du joueur dans la direction de la souris
		Vector2f startPosition = getSprite().getPosition();
		Vector2i mousePosition = Mouse::getPosition(window);
		Vector2f direction = Vector2f(mousePosition) - startPosition;
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		direction /= length; // Normaliser la direction
		grapple.launch(startPosition, direction);
	}
}

void Player::draw(RenderWindow& window)
{
	window.draw(getSprite());
	grapple.draw(window);
}