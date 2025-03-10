#include "../include/Player.hpp"

Player::Player(Map& map)
	: Entity(), grapple(500.0f), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false)
{
	speed = 200;
	velocity = Vector2f(0, 0);
	this->map = map;
}

Player::Player(const Vector2f& size, const Color& color, Map& map)
	: Entity(size, color), grapple(500.0f), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false)
{
	speed = 450;
	velocity = Vector2f(0, 0);
	this->map = map;
}


Player::~Player()
{
}

void Player::update(float dt)
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

        if (Keyboard::isKeyPressed(Keyboard::Q) && getSprite().getPosition().x - speed * dt) {
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

		isColliding(getSpriteConst().getPosition().x, getSpriteConst().getPosition().y, dt);

        getSprite().move(velocity * dt);

		cout << velocity.x << " " << velocity.y << endl;
		cout << getSpriteConst().getPosition().x << " " << getSpriteConst().getPosition().y << endl;

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
		isColliding(getSpriteConst().getPosition().x, getSpriteConst().getPosition().y, dt);
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
	if (Mouse::isButtonPressed(Mouse::Left))
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

void Player::isColliding(int x, int y, float dt)
{
	int newX = getSpriteConst().getPosition().x + velocity.x * dt;
	int newY = getSpriteConst().getPosition().y + velocity.y * dt;

	// Vérifie la collision avant d'appliquer le mouvement
	if (map.isColliding(newX, getSpriteConst().getPosition().y))
	{
		if (velocity.x > 0)
		{
			newX = (newX / 32) * 32;
            getSprite().setPosition(newX - 0.1, getSpriteConst().getPosition().y);
		}
		else if (velocity.x < 0)
		{
			newX = (newX / 32) * 32 + 32;
            getSprite().setPosition(newX + 0.1, getSpriteConst().getPosition().y);
		}
		velocity.x = 0;
	}
	if (map.isColliding(getSpriteConst().getPosition().x, newY))
	{
		if (velocity.y > 0)
		{
			newY = (newY / 32) * 32;
			jumpNum = 0;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY - 0.1);
		}
		else if (velocity.y < 0)
		{
			newY = (newY / 32) * 32 + 32;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY + 0.1);
		}

		velocity.y = 0;
	}
}
