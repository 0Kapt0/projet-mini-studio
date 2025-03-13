#include "../include/Player.hpp"

Player::Player(Map& map)
    : Entity(), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f)
{
    speed = 200;
    velocity = Vector2f(0, 0);
    this->map = map;
}

Player::Player(const Vector2f& size, const Color& color, Map& map)
    : Entity(size, color), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f)
{
    speed = 450;
    velocity = Vector2f(0, 0);
    this->map = map;
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
    handleGrapplePull(dt);
    handleMovement(dt);
    handleCollisions(dt);
    applyMovement(dt);
    updateGrapplePosition();
}



void Player::handleGrapplePull(float dt)
{
    if (grapple.isStuck() && leftButtonHold)
    {
        Vector2f playerPos = getSprite().getPosition();
        Vector2f stuckPos = grapple.getStuckPosition();

        Vector2f direction = stuckPos - playerPos;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0.f) direction /= length;

        velocity = direction * speed * 1.9f;

        grappleMove = true;
        onGround = false;
    }
}


void Player::handleMovement(float dt)
{
    if (grappleMove) return;

    if (!dashing)
    {
        handleNormalMovement(dt);
    }
    else
    {
        handleDashingMovement(dt);
    }
}


void Player::handleNormalMovement(float dt)
{
    dashTimer += dt;
    if (dashTimer >= dashCooldown && getSprite().getPosition().y > 200.f)
        canDash = true;

    if (!Keyboard::isKeyPressed(Keyboard::Z) && jumpNum < 2)
    {
        canJump = true;
    }

    velocity.y += 14.8f;

    if (getSprite().getPosition().y > 800.f)
    {
        canJump = true;
        onGround = true;
        jumpNum = 0;
        velocity.y = 0.f;
    }

    if (!Keyboard::isKeyPressed(Keyboard::Q) && !Keyboard::isKeyPressed(Keyboard::D))
        velocity.x = 0.f;

    if (Keyboard::isKeyPressed(Keyboard::Q))
    {
        velocity.x = -speed;
        lastInputDirection = 'L';
    }
    else if (Keyboard::isKeyPressed(Keyboard::D))
    {
        velocity.x = speed;
        lastInputDirection = 'R';
    }

    if (Keyboard::isKeyPressed(Keyboard::Z) && canJump)
    {
        onGround = false;
        velocity.y = -speed;
        jumpNum++;
        canJump = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::S) && getSprite().getPosition().y < 800.f)
    {
        velocity.y += speed / 4.f;
    }

    if (grapple.isStuck())
    {
        if (!grappleStuck)
        {
            grappleStuck = true;
            angle = calculateAngle(getSprite().getPosition(), grapple.getStuckPosition());
            angularVelocity = 0.f;
        }

        float gravityEffect = -swingForce * sin(angle);
        angularVelocity += gravityEffect * dt;
        angularVelocity *= DAMPING;

        if (Keyboard::isKeyPressed(Keyboard::Q) && angularVelocity > -2.0f)
            angularVelocity -= swingAcceleration * dt;
        if (Keyboard::isKeyPressed(Keyboard::D) && angularVelocity < 2.0f)
            angularVelocity += swingAcceleration * dt;

        angle += angularVelocity * dt;

        Vector2f newPos(
            grapple.getStuckPosition().x + sin(angle) * grapple.getGrappleLength(),
            grapple.getStuckPosition().y + cos(angle) * grapple.getGrappleLength()
        );

        Vector2f playerPos = getSprite().getPosition();
        float dist = std::hypot(newPos.x - playerPos.x, newPos.y - playerPos.y);
        if (dist > 20.f)
        {
            Vector2f dir = (newPos - playerPos);
            dir /= dist;
            newPos = playerPos + dir * dt * speed;
        }

        isSwingColliding(newPos, dt);

        getSprite().setPosition(newPos);
        velocity = Vector2f(0.f, 0.f);
    }
    else
    {
        grappleStuck = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Space) && canDash)
    {
        dashing = true;
        canDash = false;
        dashTimer = 0.f;
        velocity.y = 0.f;

        grapple.setActive(false);
        grapple.setStuck(false);

        switch (lastInputDirection)
        {
        case 'L': dashDirection.x = -speed * 3.f; break;
        case 'R': dashDirection.x = speed * 3.f; break;
        default:  dashDirection.x = 0.f;          break;
        }
    }
}

void Player::handleDashingMovement(float dt)
{
    dashDuration += dt;
    if (dashDuration >= 0.2f)
    {
        dashing = false;
        dashDuration = 0.f;
    }

    isColliding(getSpriteConst().getPosition().x,
        getSpriteConst().getPosition().y,
        dt * 3.f);

    getSprite().move(dashDirection.x * dt, 0.f);

    velocity.x = 0.f;
    velocity.y = 0.f;
}

void Player::handleCollisions(float dt)
{
    isColliding(getSpriteConst().getPosition().x,
        getSpriteConst().getPosition().y,
        dt);
}

void Player::applyMovement(float dt)
{
    getSprite().move(velocity * dt);
}

void Player::updateGrapplePosition()
{
    if (grapple.isActive())
    {
        grapple.updateStartPosition(getSprite().getPosition());
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
    window.draw(getSprite());
    grapple.draw(window);
}

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
            map.isColliding(getSprite().getPosition().x, newPos.y + getSprite().getGlobalBounds().height / 2))
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