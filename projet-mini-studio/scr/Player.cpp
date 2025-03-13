#include "../include/Player.hpp"
#define TILE_SIZE 64


Player::Player(Map& map)
    : Entity(), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f), rigidbody(1.0f, 0.99f, map) // Initialisez grappleLength � 0.0f
{
    speed = 200;
    velocity = Vector2f(0, 0);
    this->map = map;
}

Player::Player(const Vector2f& size, const Color& color, Map& map)
    : Entity(size, color), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f), rigidbody(1.0f, 0.99f, map) // Initialisez grappleLength � 0.0f
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
    Rigidbody2D rigidbody(1.0f, 0.99f, map);
    rigidbody.setPosition(Vector2f(100, 100));
}

Player::~Player()
{
}


Vector2f findTangentPoint(const Vector2f& center, float radius, const Vector2f& point) {
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < radius) 
    {
        return Vector2f(0, 0);
    }

    float angle = acos(radius / distance);
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    Vector2f tangent1(
        center.x + radius * (dx * cosAngle - dy * sinAngle) / distance,
        center.y + radius * (dx * sinAngle + dy * cosAngle) / distance
    );

    Vector2f tangent2(
        center.x + radius * (dx * cosAngle + dy * sinAngle) / distance,
        center.y + radius * (-dx * sinAngle + dy * cosAngle) / distance
    );

    return tangent1;
}

float calculateAngle(const Vector2f& point1, const Vector2f& point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;

    // Calculer le produit scalaire et le produit vectoriel
    float dot = point1.x * point2.x + point1.y * point2.y;
    float cross = point1.x * point2.y - point1.y * point2.x;

    // Calculer l'angle en radians
    float angle = atan(cross / dot);

    return angle;
}

void Player::update(float dt)
{
    rigidbody.update(dt);
    handleCollisionsAndSync();
    handleMovement(dt);
    handleJump(dt);
    handleGrapple(dt);
    handleDash(dt);
    handleAttack(dt);
    updateCamera();
}

void Player::updateCamera()
{
    Vector2f playerPosition = getSprite().getPosition();
    Vector2f cameraPosition = playerView.getCenter();

    cameraPosition.x = playerPosition.x;

    if (playerPosition.y > 670) {
        cameraPosition.y = 670;
    }
    else {
        cameraPosition.y = playerPosition.y;
    }

    playerView.setCenter(cameraPosition);
}

void Player::handleMovement(float dt) {
    if (Keyboard::isKeyPressed(Keyboard::Q)) {
        rigidbody.applyForce(Vector2f(-speed, 0));
        lastInputDirection = 'L';
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        rigidbody.applyForce(Vector2f(speed, 0));
        lastInputDirection = 'R';
    }
}


void Player::handleJump(float dt)
{
    if (Keyboard::isKeyPressed(Keyboard::Z) && canJump) {
        rigidbody.applyForce(Vector2f(0, -speed * 100));
        jumpNum++;
        canJump = false;
    }
}

void Player::handleGrapple(float dt)
{
    grappleMove = false;
    if (grapple.isStuck() && leftButtonHold) {
        Vector2f direction = grapple.getStuckPosition() - getSprite().getPosition();
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;
        velocity = direction * speed * 1.9f;
        grappleMove = true;
    }
}

void Player::handleDash(float dt)
{
    Vector2f playerPosition = getSprite().getPosition();

    if (Keyboard::isKeyPressed(Keyboard::Space) && canDash) {
        dashing = true;
        canDash = false;
        dashTimer = 0;
        velocity.y = 0;
        angle += angularVelocity * dt;

                Vector2f newPos = Vector2f(grapple.getStuckPosition().x + sin(angle) * grapple.getGrappleLength(),
                    grapple.getStuckPosition().y + cos(angle) * grapple.getGrappleLength());

				if (sqrt(pow(newPos.x - playerPosition.x, 2) + pow(newPos.y - playerPosition.y, 2)) > 20) 
                {
                    Vector2f direction = newPos - playerPosition;
					float length = sqrt(direction.x * direction.x + direction.y * direction.y);
					direction /= length;
					newPos = playerPosition + direction * dt * speed;
				}

				isSwingColliding(newPos, dt);

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
				grapple.setActive(false);
				grapple.setStuck(false);

        switch (lastInputDirection) {
        case 'L': dashDirection.x = -speed * 3.5f; break;
        case 'R': dashDirection.x = speed * 3.5f; break;
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
        getSprite().move(dashDirection.x * dt, 0);
    }
}


void Player::handleAttack(float dt)
{
    if (Mouse::isButtonPressed(Mouse::Left) && canAttack) {
        attacking = true;
        canAttack = false;
        attackTimer = 0;

        switch (lastInputDirection) {
        case 'L': attackDirection = "left"; break;
        case 'R': attackDirection = "right"; break;
        }
    }

    attackTimer += dt;
    if (attackTimer >= attackCooldown) canAttack = true;

    if (attacking) {
        attackDuration += dt;
        if (attackDuration >= 0.2) {
            attacking = false;
            attackDuration = 0;
        }
    }
}

void Player::handleCollisionsAndSync() {
    Vector2f newPosition = rigidbody.getPosition();

    bool wasOnGround = canJump;
    canJump = rigidbody.isOnGround();

    if (!wasOnGround && canJump) {
        velocity.x *= 0.8f;
    }

    rigidbody.setPosition(newPosition);
    getSprite().setPosition(rigidbody.getPosition());
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
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
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

    //RectangleShape hitboxDebug(Vector2f(50, 50));
    //hitboxDebug.setPosition(rigidbody.getPosition());
    //hitboxDebug.setFillColor(Color(255, 0, 0, 100));
    //window.draw(hitboxDebug);
void Player::isColliding(int x, int y, float dt)
{
    int newX = getSpriteConst().getGlobalBounds().left + velocity.x * dt;
    int newY = getSpriteConst().getGlobalBounds().top + velocity.y * dt;

    if (velocity != Vector2f(0, 0))
    {
        int newX = getSpriteConst().getGlobalBounds().left + velocity.x * dt;
        int newY = getSpriteConst().getGlobalBounds().top + velocity.y * dt;
        
        if (velocity.x > 0)
        {
            if (map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
            {
                newX = (static_cast<int>((getSprite().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width) / 32)) * 32 + 32 - getTexture().getSize().x / 2;
                getSprite().setPosition(newX - 0.1, getSpriteConst().getPosition().y);
                velocity.x = 0;
                jumpNum = 1;
            }
        }
        else if (velocity.x < 0)
        {
            if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
            {
                newX = (static_cast<int>(getSprite().getGlobalBounds().left / 32)) * 32 + getTexture().getSize().x / 2;
                getSprite().setPosition(newX + 0.1, getSpriteConst().getPosition().y);
                velocity.x = 0;
                jumpNum = 1;
            }
        }

        if (velocity.y > 0)
        {
            if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY + getSpriteConst().getGlobalBounds().height))
            {
                newY = (static_cast<int>((getSprite().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) / 32)) * 32 + 32 - getTexture().getSize().y / 2;
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
                newY = (static_cast<int>(getSprite().getGlobalBounds().top / 32)) * 32 + getTexture().getSize().y / 2;
                getSprite().setPosition(getSpriteConst().getPosition().x, newY + 0.1);
                velocity.y = 0;
                dashDirection.y = 0;
            }
        }
    }

    if (dashing)
    {
        int newX = getSpriteConst().getGlobalBounds().left + dashDirection.x * dt;
        
        if (dashDirection.x > 0)
        {
			if (map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
			{
				newX = (static_cast<int>((getSprite().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width) / 32)) * 32 + 32 - getTexture().getSize().x / 2;
				getSprite().setPosition(newX - 0.1, getSpriteConst().getPosition().y);
                dashDirection.x = 0;
				jumpNum = 1;
			}
		}
        else if (dashDirection.x < 0)
        {
            if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
            {
                newX = (static_cast<int>(getSprite().getGlobalBounds().left / 32)) * 32 + getTexture().getSize().x / 2;
                getSprite().setPosition(newX + 0.1, getSpriteConst().getPosition().y);
                dashDirection.x = 0;
                jumpNum = 1;
            }
        }
    }
}

void Player::isSwingColliding(Vector2f& newPos, float dt)
{
	Vector2f swingVelocity = newPos - getSprite().getPosition();
    
    if (swingVelocity.y > 0)
    {
        if (map.isColliding(getSprite().getPosition().x - getSprite().getGlobalBounds().width / 2, newPos.y + getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(getSprite().getPosition().x + getSprite().getGlobalBounds().height / 2, newPos.y + getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(getSprite().getPosition().x , newPos.y + getSprite().getGlobalBounds().height / 2))
        {
            newPos.y = (static_cast<int>((getSprite().getGlobalBounds().top + getSprite().getGlobalBounds().height) / 32)) * 32 + 32 - getTexture().getSize().y / 2;
            getSprite().setPosition(getSprite().getPosition().x, newPos.y - 0.1);
            velocity.y = 0;
            angularVelocity = 0;
            newPos.y = getSprite().getPosition().y;
			swingVelocity.x = 0;
        }
    }
    else if (swingVelocity.y < 0)
    {
        if (map.isColliding(getSprite().getPosition().x - getSprite().getGlobalBounds().width / 2, newPos.y - getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(getSprite().getPosition().x + getSprite().getGlobalBounds().height / 2, newPos.y - getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(getSprite().getPosition().x, newPos.y - getSprite().getGlobalBounds().height / 2))
        {
            newPos.y = (static_cast<int>(getSprite().getGlobalBounds().top / 32)) * 32 + getTexture().getSize().y / 2;
            getSprite().setPosition(getSprite().getPosition().x, newPos.y + 0.1);
            velocity.y = 0;
            angularVelocity = 0;
            newPos.y = getSprite().getPosition().y;

        }
    }

    if (swingVelocity.x > 0)
    {
        if (map.isColliding(newPos.x + getSprite().getGlobalBounds().width / 2, getSprite().getPosition().y - getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(newPos.x + getSprite().getGlobalBounds().width / 2, getSprite().getPosition().y + getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(newPos.x + getSprite().getGlobalBounds().width / 2, getSprite().getPosition().y))
        {
            newPos.x = (static_cast<int>((getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width) / 32)) * 32 + 32 - getTexture().getSize().x / 2;
            getSprite().setPosition(newPos.x - 0.1, getSprite().getPosition().y);
            velocity.x = 0;
            jumpNum = 1;
            angularVelocity = 0;
            newPos.x = getSprite().getPosition().x;
        }
    }
    else if (swingVelocity.x < 0)
    {
        if (map.isColliding(newPos.x - getSprite().getGlobalBounds().width / 2, getSprite().getPosition().y - getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(newPos.x - getSprite().getGlobalBounds().width / 2, getSprite().getPosition().y + getSprite().getGlobalBounds().height / 2) ||
            map.isColliding(newPos.x - getSprite().getGlobalBounds().width / 2, getSprite().getPosition().y))
        {
            newPos.x = (static_cast<int>(getSprite().getGlobalBounds().left / 32)) * 32 + getTexture().getSize().x / 2;
            getSprite().setPosition(newPos.x + 0.1, getSprite().getPosition().y);
            velocity.x = 0;
            jumpNum = 1;
            angularVelocity = 0;
            newPos.x = getSprite().getPosition().x;

        }
    }
}