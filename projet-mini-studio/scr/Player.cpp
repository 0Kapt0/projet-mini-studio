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

    attackTexture.create(size.x * 1.5f, size.y * 1.5f);
    Image image;
    image.create(size.x * 1.5f, size.y * 1.5f, Color::Blue);
    attackTexture.update(image);
    attackSprite.setTexture(attackTexture);
}


Player::~Player()
{
}

void Player::update(float dt)
{
    if (!dashing) {
        dashTimer += dt;
        if (dashTimer >= dashCooldown && getSprite().getPosition().y > 800) canDash = true;

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
            dashMomentum = false;
        }
        if (!Keyboard::isKeyPressed(Keyboard::Q) && !Keyboard::isKeyPressed(Keyboard::D)) {
            if (dashMomentum) {
                if (lastInputDirection == 'L') {
                    velocity.x += 14.8f;
                }
                if (lastInputDirection == 'R') {
                    velocity.x -= 14.8f;
                }
                if (velocity.x > -50 && velocity.x < 50) {
                    velocity.x = 0;
                    dashMomentum = false;
                }
            }
            else {
                velocity.x = 0;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Q) && getSprite().getPosition().x - speed * dt) {
            velocity.x = -speed;
            lastInputDirection = 'L';
            dashMomentum = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            velocity.x = speed;
            lastInputDirection = 'R';
            dashMomentum = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::Z) && canJump)
        {
            velocity.y = -speed;
            jumpNum++;
            canJump = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::S) && getSprite().getPosition().y < 800)
            velocity.y += speed;

		isColliding(getSpriteConst().getPosition().x, getSpriteConst().getPosition().y, dt);

        getSprite().move(velocity * dt);

		cout << velocity.x << " " << velocity.y << endl;

        if (Keyboard::isKeyPressed(Keyboard::Space) && canDash) {
            dashing = true;
            canDash = false;
            dashTimer = 0;
            velocity.y = 0;

            switch (lastInputDirection) {
            case('L'): dashDirection.x = -speed * 3.5f; break;
            case('R'): dashDirection.x = speed * 3.5f; break;
            }
            //dashDirection = velocity * 3.f; // si on veut un dash dans toutes les directions
        }
    }

    if (dashing) {
        dashDuration += dt;
        if (dashDuration >= 0.1) {
            dashing = false;
            dashDuration = 0;
            dashMomentum = true;
            velocity = dashDirection;
        }
		isColliding(getSpriteConst().getPosition().x, getSpriteConst().getPosition().y, dt);
        getSprite().move(dashDirection.x * dt, 0);
    }
    if (grapple.isActive()) {
        grapple.updateStartPosition(getSprite().getPosition());
    }
    // Mettre à jour le grappin
    grapple.update(dt);

    if (Mouse::isButtonPressed(Mouse::Left) /*Keyboard::isKeyPressed(Keyboard::F)*/ && canAttack) {
        attacking = true;
        canAttack = false;
        attackTimer = 0;

        switch (lastInputDirection) {
            case('L'): attackDirection = "left"; break;
            case('R'): attackDirection = "right"; break;
        }
    }
    attackTimer += dt;
    if (attackTimer >= attackCooldown) canAttack = true;

    if (attacking) {
        attackDuration += dt;
        if (attackDuration >= 0.2){
            attacking = false;
            attackDuration = 0;
        }
        if (attackDirection == "left"){
            attackSprite.setPosition(getSprite().getPosition().x - (getSprite().getLocalBounds().width * getSprite().getScale().x) / 2
                - (attackSprite.getLocalBounds().width * attackSprite.getScale().x),
                getSprite().getPosition().y - (attackSprite.getLocalBounds().width * attackSprite.getScale().x) / 2);
        }
        if (attackDirection == "right") {
            attackSprite.setPosition(getSprite().getPosition().x + (getSprite().getLocalBounds().width * getSprite().getScale().x) / 2,
                getSprite().getPosition().y - (attackSprite.getLocalBounds().width * attackSprite.getScale().x) / 2);
        }
    }
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
    if (attacking) window.draw(attackSprite);
}

void Player::isColliding(int x, int y, float dt)
{
	int newX = getSpriteConst().getPosition().x + velocity.x * dt;
	int newY = getSpriteConst().getPosition().y + velocity.y * dt;

	// Vérifie la collision avant d'appliquer le mouvement
	if (map.isColliding(newX, newY))
	{
        velocity.x = 0;
		velocity.y = 0;
	}
}
