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

    if (distance < radius) {
        // Pas de tangente possible
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

        Vector2f newPos = Vector2f(grapple.getStuckPosition().x + (angle)*grapple.getGrappleLength(),
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
    else {
        grappleStuck = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::LShift) && canDash) {
        dashing = true;
        canDash = false;
        dashTimer = 0;
        velocity.y = 0;

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

    RectangleShape hitboxDebug(Vector2f(50, 50));
    hitboxDebug.setPosition(rigidbody.getPosition());
    hitboxDebug.setFillColor(Color(255, 0, 0, 100));
    window.draw(hitboxDebug);

}
