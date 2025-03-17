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
    hp = 1;
    cout << hp;
    speed = 450;
    velocity = Vector2f(0, 0);
    this->map = map;
    playerView.setSize(1920, 1080);
    attackTexture.create(size.x * 1.5f, size.y * 1.5f);
    Image image;
    image.create(size.x * 1.5f, size.y * 1.5f, Color::Blue);
    attackTexture.update(image);
    attackSprite.setTexture(attackTexture);
    if (!heartTexure.loadFromFile("assets/ui/heart.png")) {
        cerr << "Erreur lors du chargement du coeur." << endl;
    }
    heart1.setTexture(heartTexure);
    if (!heartemptyTexure.loadFromFile("assets/ui/heartempty.png")) {
        cerr << "Erreur lors du chargement du coeur." << endl;
    }
    heartempty.setTexture(heartemptyTexure);
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

    float dot = point1.x * point2.x + point1.y * point2.y;
    float cross = point1.x * point2.y - point1.y * point2.x;

    float angle = atan(cross / dot);

    return angle;
}

void Player::update(float dt)
{
    invincibilityAfterHit(dt);
    handleGrapplePull(dt);
    handleMovement(dt);
    handleAttack(dt);
    handleCollisions(dt);
    applyMovement(dt);
    updateGrapplePosition();
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
    if (onGround) dashMomentum = false;
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
    if (dashTimer >= dashCooldown && onGround)
        canDash = true;

    if (!Keyboard::isKeyPressed(Keyboard::Space) && jumpNum < 2)
    {
        canJump = true;
    }

    velocity.y += 14.8f;

    if (getSprite().getPosition().y > 1100.f)
    {
        canJump = true;
        onGround = true;
        jumpNum = 0;
        velocity.y = 0.f;
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

    if (Keyboard::isKeyPressed(Keyboard::Q))
    {
        velocity.x = -speed;
        lastInputDirection = 'L';
        dashMomentum = false;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D))
    {
        velocity.x = speed;
        lastInputDirection = 'R';
        dashMomentum = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Space) && canJump)
    {
        onGround = false;
        velocity.y = -speed;
        jumpNum++;
        canJump = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::S) && getSprite().getPosition().y < 1100.f)
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

    if (Keyboard::isKeyPressed(Keyboard::LShift) && canDash)
    {
        dashing = true;
        canDash = false;
        dashTimer = 0.f;
        velocity.y = 0.f;

        grapple.setActive(false);
        grapple.setStuck(false);

        switch (lastInputDirection)
        {
        case 'L': dashDirection.x = -speed * 3.5f; break;
        case 'R': dashDirection.x = speed * 3.5f; break;
        default:  dashDirection.x = 0.f;          break;
        }
    }
}

void Player::handleDashingMovement(float dt)
{
    dashDuration += dt;
    if (dashDuration >= 0.1f)
    {
        dashing = false;
        dashDuration = 0.f;
        dashMomentum = true;
        velocity = dashDirection;
    }

    isColliding(getSpriteConst().getPosition().x,
        getSpriteConst().getPosition().y,
        dt * 3.f);

    getSprite().move(dashDirection.x * dt, 0.f);

    /*velocity.x = 0.f;
    velocity.y = 0.f;*/
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
    if (/*Mouse::isButtonPressed(Mouse::Left)*/ Keyboard::isKeyPressed(Keyboard::F)) {
        leftButtonHold = true;
        if (grapple.isStuck()) 
        {
            Vector2f stuckPosition = grapple.getStuckPosition();
            Vector2f playerPosition = getSprite().getPosition();
            Vector2f direction = stuckPosition - playerPosition;
            grapple.setGrappleLength(sqrt(direction.x * direction.x + direction.y * direction.y));
            direction /= grappleLength;
            velocity = direction * speed;
        }
        else 
        {
            Vector2f startPosition = getSprite().getPosition();
            Vector2i mousePosition = Mouse::getPosition(window);
            Vector2f worldPosition = window.mapPixelToCoords(mousePosition, playerView);
            Vector2f direction = worldPosition - startPosition;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= length;
            grapple.launch(startPosition, direction);
        }
    }
    else 
    {
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

void Player::isColliding(int x, int y, float dt)
{
    handleBoundingBoxCollision(dt);

    handleDashingCollision(dt);

    bool slopeMoved = true;
    int iterations = 0;
    const int MAX_SLOPE_ITERATIONS = 5;

    while (slopeMoved && iterations < MAX_SLOPE_ITERATIONS)
    {
        slopeMoved = handleSlopeCollisionOnce();
        iterations++;
    }
}


void Player::handleBoundingBoxCollision(float dt)
{
    if (velocity == sf::Vector2f(0, 0)) {
        return;
    }

    int newX = getSpriteConst().getGlobalBounds().left + velocity.x * dt;
    int newY = getSpriteConst().getGlobalBounds().top + velocity.y * dt;

    if (velocity.x > 0)
    {
        if (map.isColliding(newX + getSpriteConst().getGlobalBounds().width,
            getSpriteConst().getGlobalBounds().top) ||
            map.isColliding(newX + getSpriteConst().getGlobalBounds().width,
                getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) ||
            map.isColliding(newX + getSpriteConst().getGlobalBounds().width,
                getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>((getSprite().getGlobalBounds().left
                + getSpriteConst().getGlobalBounds().width) / 64)) * 64
                + 64
                - getTexture().getSize().x / 2;

            getSprite().setPosition(newX - 0.1f, getSpriteConst().getPosition().y);
            velocity.x = 0.f;
            jumpNum = 1;
        }
    }
    else if (velocity.x < 0)
    {
        if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) ||
            map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) ||
            map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(getSprite().getGlobalBounds().left / 64)) * 64
                + getTexture().getSize().x / 2;

            getSprite().setPosition(newX + 0.1f, getSpriteConst().getPosition().y);
            velocity.x = 0.f;
            jumpNum = 1;
        }
    }

    if (velocity.y > 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left,
            newY + getSpriteConst().getGlobalBounds().height) ||
            map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width,
                newY + getSpriteConst().getGlobalBounds().height) ||
            map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2,
                newY + getSpriteConst().getGlobalBounds().height))
        {
            newY = (static_cast<int>((getSprite().getGlobalBounds().top
                + getSpriteConst().getGlobalBounds().height) / 64)) * 64
                + 64
                - getTexture().getSize().y / 2;

            jumpNum = 0;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY - 0.1f);
            velocity.y = 0.f;
            dashDirection.y = 0.f;
            onGround = true;
        }
    }
    else if (velocity.y < 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY) ||
            map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY) ||
            map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY))
        {
            newY = (static_cast<int>(getSprite().getGlobalBounds().top / 64)) * 64
                + getTexture().getSize().y / 2;

            getSprite().setPosition(getSpriteConst().getPosition().x, newY + 0.1f);
            velocity.y = 0.f;
            dashDirection.y = 0.f;
        }
    }
}

void Player::handleDashingCollision(float dt)
{
    if (!dashing) {
        return;
    }

    int newX = getSpriteConst().getGlobalBounds().left + dashDirection.x * dt;

    if (dashDirection.x > 0)
    {
        if (map.isColliding(
            newX + getSpriteConst().getGlobalBounds().width,
            getSpriteConst().getGlobalBounds().top) ||
            map.isColliding(
                newX + getSpriteConst().getGlobalBounds().width,
                getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) ||
            map.isColliding(
                newX + getSpriteConst().getGlobalBounds().width,
                getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(
                (getSprite().getGlobalBounds().left
                    + getSpriteConst().getGlobalBounds().width) / 64)) * 64
                + 64
                - getTexture().getSize().x / 2;

            getSprite().setPosition(newX - 0.1f, getSpriteConst().getPosition().y);
            dashDirection.x = 0.f;
            jumpNum = 1;
        }
    }
    else if (dashDirection.x < 0)
    {
        if (map.isColliding(
            newX,
            getSpriteConst().getGlobalBounds().top) ||
            map.isColliding(
                newX,
                getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) ||
            map.isColliding(
                newX,
                getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(
                getSprite().getGlobalBounds().left / 64)) * 64
                + getTexture().getSize().x / 2;

            getSprite().setPosition(newX + 0.1f, getSpriteConst().getPosition().y);
            dashDirection.x = 0.f;
            jumpNum = 1;
        }
    }
}

bool Player::handleSlopeCollisionOnce()
{
    float left = getSpriteConst().getGlobalBounds().left;
    float right = left + getSpriteConst().getGlobalBounds().width;
    float bottom = getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height;

    float cornerX = left;

    int tileX = static_cast<int>(cornerX / 64);
    int tileY = static_cast<int>(bottom / 64);

    SlopeType slopeType = map.getSlopeTypeAt(tileX, tileY);

    if (slopeType == SlopeType::SlopeUp) {
        cornerX = right;
    }
    else if (slopeType == SlopeType::SlopeDown) {
        cornerX = left;
    }
    else {
        return false;
    }

    tileX = static_cast<int>(cornerX / 64);
    float slopeY = map.getSlopeSurfaceY(tileX, tileY, cornerX);

    if (bottom > slopeY)
    {
        float delta = bottom - slopeY;

        float correctionSpeed = 0.1f;
        float correction = std::min(delta, correctionSpeed * 64);

        float horizontalAdjust = 0.f;
        if (slopeType == SlopeType::SlopeUp) {
            horizontalAdjust = -correction * 0.5f;
        }

        getSprite().move(horizontalAdjust, -correction);
        velocity.y = 0.f;
        onGround = true;

        return true;
    }

    return false;
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
            newPos.y = (static_cast<int>((getSprite().getGlobalBounds().top + getSprite().getGlobalBounds().height) / 64)) * 64 + 64 - getTexture().getSize().y / 2;
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
            newPos.y = (static_cast<int>(getSprite().getGlobalBounds().top / 64)) * 64 + getTexture().getSize().y / 2;
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
            newPos.x = (static_cast<int>((getSprite().getGlobalBounds().left + getSprite().getGlobalBounds().width) / 64)) * 64 + 64 - getTexture().getSize().x / 2;
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
            newPos.x = (static_cast<int>(getSprite().getGlobalBounds().left / 64)) * 64 + getTexture().getSize().x / 2;
            getSprite().setPosition(newPos.x + 0.1, getSprite().getPosition().y);
            velocity.x = 0;
            jumpNum = 1;
            angularVelocity = 0;
            newPos.x = getSprite().getPosition().x;

        }
    }
}

void Player::draw(RenderWindow& window)
{
    window.setView(playerView);
    //heart1
    for (int i = 1; i < 4; i++) {
        heartempty.setPosition(-1050 + (i * 100) + window.getView().getCenter().x, -530 + window.getView().getCenter().y);
        heartempty.setScale(0.1, 0.1f);
        window.draw(heartempty);
        if (hp >= i) {
            heart1.setPosition(-1050 + (i * 100) + window.getView().getCenter().x, -530 + window.getView().getCenter().y);
            heart1.setScale(0.1, 0.1f);
            window.draw(heart1);
        }
    }
    window.draw(getSprite());
    if (attacking)
        window.draw(attackSprite);
    grapple.draw(window);
}

void Player::handleAttack(float dt) {
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

    //attackSprite.setPosition(getSprite().getPosition());
    if (attacking) {
        attackDuration += dt;
        if (attackDuration >= 0.2) {
            attacking = false;
            attackDuration = 0;
        }
    }
    if (attackDirection == "left") {
        attackSprite.setPosition(getSprite().getPosition().x - getWidth() / 2
            - (attackSprite.getLocalBounds().width * attackSprite.getScale().x),
            getSprite().getPosition().y - (attackSprite.getLocalBounds().width * attackSprite.getScale().x) / 2);
        lastAttackPosition = attackSprite.getPosition();
    }
    if (attackDirection == "right") {
        attackSprite.setPosition(getSprite().getPosition().x + getWidth() / 2,
            getSprite().getPosition().y - (attackSprite.getLocalBounds().width * attackSprite.getScale().x) / 2);
        lastAttackPosition = attackSprite.getPosition();
    }
}

Sprite Player::getAttackHitBox() {

    return attackSprite;
}
