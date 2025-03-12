#include "../include/Player.hpp"
#define TILE_SIZE 64


Player::Player(Map& map)
    : Entity(), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f) // Initialisez grappleLength à 0.0f
{
    speed = 200;
    velocity = Vector2f(0, 0);
    this->map = map;
}

Player::Player(const Vector2f& size, const Color& color, Map& map)
    : Entity(size, color), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f) // Initialisez grappleLength à 0.0f
{
    speed = 450;
    velocity = Vector2f(0, 0);
    this->map = map;
    attackTexture.create(size.x * 1.5f, size.y * 1.5f);
    Image image;
    image.create(size.x * 1.5f, size.y * 1.5f, Color::Blue);
    attackTexture.update(image);
    attackSprite.setTexture(attackTexture);
    playerView.setSize(1920, 1080);
}

Player::~Player()
{
}


Vector2f findTangentPoint(const Vector2f& center, float radius, const Vector2f& point) {
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < radius) {
        // Pas de tangente possible
        return Vector2f(0, 0);
    }

    float angle = std::acos(radius / distance);
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);

    Vector2f tangent1(
        center.x + radius * (dx * cosAngle - dy * sinAngle) / distance,
        center.y + radius * (dx * sinAngle + dy * cosAngle) / distance
    );

    Vector2f tangent2(
        center.x + radius * (dx * cosAngle + dy * sinAngle) / distance,
        center.y + radius * (-dx * sinAngle + dy * cosAngle) / distance
    );

    // Choisir l'un des points de tangence (par exemple, tangent1)
    return tangent1;
}

float calculateAngle(const Vector2f& point1, const Vector2f& point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;

    // Calculer le produit scalaire et le produit vectoriel
    float dot = point1.x * point2.x + point1.y * point2.y;
    float cross = point1.x * point2.y - point1.y * point2.x;

    // Calculer l'angle en radians
    float angle = std::atan(cross / dot);

    return angle;
}

void Player::update(float dt)
{
    isColliding(getSprite().getPosition().x, getSprite().getPosition().y, dt);
	grappleMove = false;
    Vector2f playerPosition = getSprite().getPosition();
    Vector2f stuckPosition = grapple.getStuckPosition();
    if (grapple.isStuck() && leftButtonHold) {
        Vector2f direction = stuckPosition - playerPosition;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length; // Normaliser la direction
        velocity = direction * speed * 1.9f;
		grappleMove = true;
        onGround = false;
    }


    if (!grappleMove)
    {
        if (!dashing) {
            dashTimer += dt;
            if (dashTimer >= dashCooldown && getSprite().getPosition().y > 200) canDash = true;

            if (!Keyboard::isKeyPressed(Keyboard::Z) && jumpNum < 2)
            {
                canJump = true;
            }

            velocity.y += 14.8f;

        if (getSprite().getPosition().y > 1150)
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
        if (Keyboard::isKeyPressed(Keyboard::S) && getSprite().getPosition().y < 1150)
            velocity.y += speed;

            if (grapple.isStuck()) {
                if (!grappleStuck)
                {
					grappleStuck = true;
                    angle = calculateAngle(playerPosition, grapple.getStuckPosition());
					angularVelocity = 0;
                }
                float gravityEffect = -swingForce * sin(angle);
                angularVelocity += gravityEffect * dt;
                angularVelocity *= DAMPING;

                if (Keyboard::isKeyPressed(Keyboard::Q) && angularVelocity > -2.0f) {
                    angularVelocity -= swingAcceleration * dt;
                }
                if (Keyboard::isKeyPressed(Keyboard::D) && angularVelocity < 2.0f) {
                    angularVelocity += swingAcceleration * dt;
                }

                angle += angularVelocity * dt;

                Vector2f newPos = Vector2f(grapple.getStuckPosition().x + (angle) * grapple.getGrappleLength(),
                    grapple.getStuckPosition().y + cos(angle) * grapple.getGrappleLength());

				if (sqrt(pow(newPos.x - playerPosition.x, 2) + pow(newPos.y - playerPosition.y, 2)) > 20) {
					Vector2f direction = newPos - playerPosition;
					float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
					direction /= length;
					newPos = playerPosition + direction * dt * speed;
				}



                getSprite().setPosition(newPos);

                velocity = Vector2f(0, 0);
            }
            else
			{
				grappleStuck = false;
			}


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
    Vector2f cameraPosition = playerView.getCenter();

    cameraPosition.x = playerPosition.x;

    if (playerPosition.y > 670) {
        cameraPosition.y = 670;
    }
    else {
        cameraPosition.y = playerPosition.y;
    }

    playerView.setCenter(cameraPosition);
    getSprite().move(velocity.x * dt, velocity.y * dt);
}

}

void Player::handleInput(const Event& event, RenderWindow& window, float dt)
{
    if (Mouse::isButtonPressed(Mouse::Left)) {
        leftButtonHold = true;
        if (grapple.isStuck()) {
            Vector2f stuckPosition = grapple.getStuckPosition();
            Vector2f playerPosition = getSprite().getPosition();
            Vector2f direction = stuckPosition - playerPosition;
            grapple.setGrappleLength(sqrt(direction.x * direction.x + direction.y * direction.y));
            direction /= grappleLength;
            velocity = direction * speed;
        }
        else {
            Vector2f startPosition = getSprite().getPosition();
            Vector2i mousePosition = Mouse::getPosition(window);
            Vector2f direction = Vector2f(mousePosition) - startPosition;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= length;
            grapple.launch(startPosition, direction);
        }
    }
    else {
        leftButtonHold = false;
    }
    if (Mouse::isButtonPressed(Mouse::Right))
    {
        if (grapple.isActive())
        {
            grapple.setActive(false);
            grapple.setStuck(false);
        }
    }

    grapple.update(dt, window);
}

void Player::draw(RenderWindow& window)
{
    window.setView(playerView);

	window.draw(getSprite());
	grapple.draw(window);
    if (attacking) window.draw(attackSprite);

}

void Player::isColliding(int x, int y, float dt)
{
    int newX = getSpriteConst().getGlobalBounds().left + velocity.x * dt;
    int newY = getSpriteConst().getGlobalBounds().top + velocity.y * dt;

    // Vérifie la collision avant d'appliquer le mouvement
    if (velocity.x > 0)
    {
        if (map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>((getSprite().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width) / 64)) * 64 + 64 - getTexture().getSize().x / 2;
            getSprite().setPosition(newX - 0.1, getSpriteConst().getPosition().y);
            velocity.x = 0;
            dashDirection.x = 0;
            jumpNum = 1;
        }
    }
    else if (velocity.x < 0)
    {
        if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(getSprite().getGlobalBounds().left / 64)) * 64 + getTexture().getSize().x / 2;
            getSprite().setPosition(newX + 0.1, getSpriteConst().getPosition().y);
            velocity.x = 0;
            dashDirection.x = 0;
            jumpNum = 1;
        }
    }

    if (velocity.y > 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY + getSpriteConst().getGlobalBounds().height))
        {
            newY = (static_cast<int>((getSprite().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) / 64)) * 64 + 64 - getTexture().getSize().y / 2;
            jumpNum = 0;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY - 0.1);
            velocity.y = 0;
            dashDirection.y = 0;
            onGround = true;
        }
    }
    else if (velocity.y < 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY))
        {
            newY = (static_cast<int>(getSprite().getGlobalBounds().top / 64)) * 64 + getTexture().getSize().y / 2;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY + 0.1);
            velocity.y = 0;
            dashDirection.y = 0;
        }
    }
}


