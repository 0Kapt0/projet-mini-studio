#include "../include/Player.hpp"

float getRightJoystickAngle(unsigned int joystickId) {
    float x = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::U); // Axe X du joystick droit
    float y = sf::Joystick::getAxisPosition(joystickId, sf::Joystick::V); // Axe Y du joystick droit

    // Calculer l'angle en radians
    float angle = std::atan2(y, x);

    // Convertir l'angle en degrés
    float angleDegrees = angle * 180 / 3.14159265f;

    return angleDegrees;
}

SoundManager& soundManager = SoundManager::getInstance();

Player::Player(Texture& tex, Map& map)
    : Entity(tex), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f)
{

    attackSprite.setTexture(tex);
    speed = 450;
    velocity = Vector2f(0, 0);
    this->map = map;
    playerView.setSize(1920, 1080);
    if (!heartTexure.loadFromFile("assets/ui/heart.png")) {
        cerr << "Erreur lors du chargement du coeur." << endl;
    }
    heart1.setTexture(heartTexure);
    if (!heartemptyTexure.loadFromFile("assets/ui/heartempty.png")) {
        cerr << "Erreur lors du chargement du coeur." << endl;
    }
    heartempty.setTexture(heartemptyTexure);

    hurtbox.setRadius(50);
    hurtbox.setFillColor(Color::Transparent);
    hurtbox.setOutlineColor(Color::Red);
    hurtbox.setOutlineThickness(1.f);

}

Player::Player(const Vector2f& size, const Color& color, Map& map)
    : Entity(size, color), grapple(500.0f, map), map(map), speed(450), velocity(Vector2f(0, 0)), canJump(true), jumpNum(0), canDash(true), dashing(false), dashDirection(Vector2f(0, 0)), lastInputDirection('N'), dashDuration(0), dashCooldown(0.8), dashTimer(0), grapplingTouched(false), leftButtonHold(false), grappleLength(0.0f)
{
    speed = 450;
    velocity = Vector2f(0, 0);
    this->map = map;
    playerView.setSize(1920, 1080);
    //attackTexture.create(size.x * 1.5f, size.y * 1.5f);
    Image image;
    image.create(size.x * 1.5f, size.y * 1.5f, Color::Blue);
    /*attackTexture.update(image);
    attackSprite.setTexture(attackTexture);*/
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
    grappleMove = false;
    hurtbox.setPosition(getSprite().getPosition().x - hurtbox.getRadius(), getSprite().getPosition().y - hurtbox.getRadius());
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
    Vector2f playerPos = getSprite().getPosition();

    Vector2f cameraPos = playerView.getCenter();
    cameraPos.x = playerPos.x;

    float cameraPosBas = 1620.f;
    float cameraPosHaut = 1620.f - 1080.f;

    float switchThreshold = 1080.f;
    if (playerPos.y < switchThreshold) {
        cameraPos.y = cameraPosHaut;
    }
    else {
        cameraPos.y = cameraPosBas;
    }

    float halfViewWidth = playerView.getSize().x * 0.5f;
    float leftLimit = 70.f + halfViewWidth;
    float rightLimit = 15389.f;

    if (cameraPos.x < leftLimit) {
        cameraPos.x = leftLimit;
    }
    if (cameraPos.x > rightLimit) {
        cameraPos.x = rightLimit;
    }

    playerView.setCenter(cameraPos);
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
    float axisX = Joystick::getAxisPosition(0, Joystick::X);

    dashTimer += dt;
    if (dashTimer >= dashCooldown && onGround)
        canDash = true;

    if ((!Keyboard::isKeyPressed(Keyboard::Space) || Joystick::isButtonPressed(0, 0)) && jumpNum < 2)
    {
        canJump = true;
    }

    velocity.y += 14.8f;

    if (getSprite().getPosition().y > 2200.f)
    {
        canJump = true;
        onGround = true;
        jumpNum = 0;
        velocity.y = 0.f;
    }

    if (!Keyboard::isKeyPressed(Keyboard::Q) && !Keyboard::isKeyPressed(Keyboard::D) || axisX > -50 && axisX < 50) 
    {
        if (dashMomentum) 
        {
            if (lastInputDirection == 'L') 
            {
                velocity.x += 14.8f;
            }
            if (lastInputDirection == 'R') 
            {
                velocity.x -= 14.8f;
            }
            if (velocity.x > -50 && velocity.x < 50) 
            {
                velocity.x = 0;
                dashMomentum = false;
            }
        }
        else
        {
            velocity.x = 0;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Q) || axisX < -50)
    {
        velocity.x = -speed;
        lastInputDirection = 'L';
        dashMomentum = false;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D) || axisX > 50)
    {
        velocity.x = speed;
        lastInputDirection = 'R';
        dashMomentum = false;
    }
    float jumpCooldownTime = 0.4f;
    if ((Keyboard::isKeyPressed(Keyboard::Space) || Joystick::isButtonPressed(0, 0)) && canJump && jumpCooldownClock.getElapsedTime().asSeconds() >= jumpCooldownTime)
    {
		soundManager.playSound("JumpingSound");
        onGround = false;
        velocity.y = -speed;
        jumpNum++;
        canJump = false;
        jumpCooldownClock.restart();
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

        if ((Keyboard::isKeyPressed(Keyboard::Q) || axisX < -50) && angularVelocity > -2.0f)
            angularVelocity -= swingAcceleration * dt;
        if ((Keyboard::isKeyPressed(Keyboard::D) || axisX > 50) && angularVelocity < 2.0f)
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
    float axisZ = Joystick::getAxisPosition(0, Joystick::Z);

    if ((Keyboard::isKeyPressed(Keyboard::LShift) || axisZ > 50) && canDash)

    {
		soundManager.playSound("Dash");
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
    float axisZ = Joystick::getAxisPosition(0, Joystick::Z);
    if (Mouse::isButtonPressed(Mouse::Right) || axisZ < -50) {
        leftButtonHold = true;
        if (grapple.isStuck() && grapple.isActive())
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
            Vector2f worldPosition;
            if (Joystick::isConnected(0))
            {
                float axisR = Joystick::getAxisPosition(0, Joystick::U);
                float axisU = Joystick::getAxisPosition(0, Joystick::V);
                Vector2f direction = Vector2f(axisR, axisU);
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                direction /= length;
                grapple.launch(startPosition, direction);
            }
            else
            {
                Vector2i mousePosition = Mouse::getPosition(window);
                Vector2f worldPosition = window.mapPixelToCoords(mousePosition, playerView);
                Vector2f direction = worldPosition - startPosition;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                direction /= length;
                grapple.launch(startPosition, direction);
            }

        }
    }
    else
    {
        leftButtonHold = false;
    }
    if ((Keyboard::isKeyPressed(Keyboard::F) || Joystick::isButtonPressed(0, 5)))
    {
        if (grapple.isActive())
        {
			soundManager.playSound("GrapleShootSound");
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
    if (velocity == sf::Vector2f(0, 0))
    {
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
                - getSprite().getTextureRect().getSize().x / 2;

            velocity.x = 0.f;
            jumpNum = 1;
        }
    }
    else if (velocity.x < 0)
    {
        if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(getSprite().getGlobalBounds().left / 64)) * 64 + getSprite().getTextureRect().getSize().x / 2;

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
            newY = (static_cast<int>((getSprite().getGlobalBounds().top)));

            jumpNum = 0;
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
                + getSprite().getTextureRect().getSize().y / 2;

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
                - /*getTexture().getSize().x / 2*/ getSprite().getTextureRect().getSize().x / 2;

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
                + /*getTexture().getSize().x / 2*/ getSprite().getTextureRect().getSize().x / 2;

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
    ////heart1
    //for (int i = 1; i < maxHp + 1; i++) {
    //    heartempty.setPosition(-1050 + (i * 100) + window.getView().getCenter().x, -530 + window.getView().getCenter().y);
    //    heartempty.setScale(0.1, 0.1f);
    //    window.draw(heartempty);
    //    if (hp >= i) {
    //        heart1.setPosition(-1050 + (i * 100) + window.getView().getCenter().x, -530 + window.getView().getCenter().y);
    //        heart1.setScale(0.1, 0.1f);
    //        window.draw(heart1);
    //    }
    //}

    /*RectangleShape HB(Vector2f(getSprite().getGlobalBounds().width, getSprite().getGlobalBounds().height));
    HB.setPosition(getSprite().getGlobalBounds().left, getSprite().getGlobalBounds().top);
    HB.setFillColor(Color::Magenta);
    window.draw(HB);*/

    window.draw(getSprite());
    if (attackHitboxActive)
        window.draw(attackSprite);
    grapple.draw(window);
}
void Player::drawHearts(RenderWindow& window)
{
    for (int i = 1; i < maxHp + 1; i++) {
        heartempty.setPosition(-1050 + (i * 100) + window.getView().getCenter().x, -530 + window.getView().getCenter().y);
        heartempty.setScale(0.1, 0.1f);
        window.draw(heartempty);
        if (hp >= i) {
            heart1.setPosition(-1050 + (i * 100) + window.getView().getCenter().x, -530 + window.getView().getCenter().y);
            heart1.setScale(0.1, 0.1f);
            window.draw(heart1);
        }
    }
}

void Player::handleAttack(float dt) {
    if ((Mouse::isButtonPressed(Mouse::Left) /*Keyboard::isKeyPressed(Keyboard::F)*/|| Joystick::isButtonPressed(0, 2)) && canAttack) {
		soundManager.playSound("MeleeAttackSound");
        attacking = true;
        canAttack = false;
        attackTimer = 0;
    }
    attackTimer += dt;
    if (attackTimer >= attackCooldown) canAttack = true;

    if (attacking) {
        attackDuration += dt;
        if (attackDuration < 0.3) {
            getSprite().setTextureRect(attackingFrames[0]);
            switch (lastInputDirection) {
            case('L'): attackDirection = "left"; break;
            case('R'): attackDirection = "right"; break;
            }
        }
        if (attackDuration >= 0.3 && attackDuration < 0.4) {

            attackHitboxActive = true;
            getSprite().setTextureRect(attackingFrames[1]);
        }
        if (attackDuration >= 0.4 && attackDuration < 0.6) {
            getSprite().setTextureRect(attackingFrames[2]);
            attackHitboxActive = false;
        }
        if (attackDuration >= 0.5) {
            attacking = false;
            attackDuration = 0;
        }
        getSprite().setOrigin(getSprite().getTextureRect().getSize().x / 2, getSprite().getTextureRect().getSize().y / 2);
    }
    if (attackDirection == "left") {
        attackSprite.setPosition(getSprite().getPosition().x - attackSprite.getGlobalBounds().width, getSprite().getPosition().y);
        attackSprite.setScale(-1.f, 1.f);
        lastAttackPosition = attackSprite.getPosition();
    }
    if (attackDirection == "right") {
        attackSprite.setPosition(getSprite().getPosition().x + attackSprite.getGlobalBounds().width, getSprite().getPosition().y);
        attackSprite.setScale(1.f, 1.f);
        lastAttackPosition = attackSprite.getPosition();
    }
}

Sprite Player::getAttackHitBox() {

    return attackSprite;
}

bool Player::isAttacking() {
    return attackHitboxActive;
}

int Player::getMaxHp() {
    return maxHp;
}
void Player::setMaxHp(int newMaxHp) {
    maxHp = newMaxHp;
}

void Player::oneUp(int value) {
    maxHp += value;
    hp += value;
}

void Player::setTexture(Texture& tex, int frameWidth, int frameHeight, int _totalFrames, float _frameTime) {

    frames.clear();
    standingFrames.clear();
    runningFrames.clear();
    jumpingFrames.clear();
    attackingFrames.clear();
    //RUNNING
    frameWidthResize = 157; // -1 par rapport aux values de base pck décalage
    frameHeightResize = 119;
    frameStartingX = 0;
    frameStartingY = 0;
    totalFrames = 4;
    for (int i = 0; i < totalFrames; i++) {
        runningFrames.emplace_back(i * frameWidthResize, 0, frameWidthResize, frameHeightResize);
    }
    //STANDING
    frameWidthResize = 49;
    frameHeightResize = 119;
    totalFrames = 3;
    for (int i = 0; i < totalFrames; i++) {
        standingFrames.emplace_back(i * frameWidthResize, 119, frameWidthResize, frameHeightResize);
    }
    //JUMPING
    frameWidthResize = 46;
    frameHeightResize = 139;
    totalFrames = 2;
    for (int i = 0; i < totalFrames; i++) {
        jumpingFrames.emplace_back(i * frameWidthResize, 238, frameWidthResize, frameHeightResize);  // METTRE i POUR FULL ANIMATION
    }
    //ATTACKING
    frameWidthResize = 157;
    frameHeightResize = 119;
    totalFrames = 3;
    for (int i = 0; i < totalFrames; i++) {
        attackingFrames.emplace_back(i * frameWidthResize, 377, frameWidthResize, frameHeightResize);
    }
    //ATTACK HITBOX
    totalFrames = 2;
    for (int i = 0; i < totalFrames; i++) {
        attackHitboxFrames.emplace_back(i * frameWidthResize, 496, frameWidthResize, frameHeightResize);
    }
    attackSprite.setTextureRect(attackHitboxFrames[0]);
    attackSprite.setOrigin(attackSprite.getTextureRect().getSize().x / 2, attackSprite.getTextureRect().getSize().y / 2);
    //FALLING
    frameWidthResize = 97;
    frameHeightResize = 119;
    totalFrames = 2;
    for (int i = 0; i < totalFrames; i++) {
        fallingFrames.emplace_back(i * frameWidthResize, 615, frameWidthResize, frameHeightResize);
    }
    fallingFrames.emplace_back(fallingFrames[0]);
    fallingFrames.emplace_back(fallingFrames[1]);

    frames = standingFrames;
    getSprite().setTextureRect(frames[currentFrame]);
    getSprite().setOrigin(getSprite().getTextureRect().getSize().x / 2, getSprite().getTextureRect().getSize().y / 2);
}

void Player::animate(float deltaTime) {
    elapsedTime += deltaTime;
    if (lastInputDirection == 'L') {
        getSprite().setScale(-1.f, 1.f);
    }
    if (lastInputDirection == 'R') {
        getSprite().setScale(1.f, 1.f);
    }
    if (elapsedTime >= frameTime && !frames.empty()) {
        elapsedTime = 0.0f;
        if (velocity.x > 0 && velocity.y <= 0) {
            if (frames != runningFrames || getSprite().getScale() != Vector2f(1.f, 1.f)) {
                currentFrame = 0; //reset l'animation
            }
            frames = runningFrames;
            totalFrames = 4;
            //if (!attacking) getSprite().setScale(1.f, 1.f);
        }
        else if (velocity.x < 0 && velocity.y <= 0) {
            if (frames != runningFrames || getSprite().getScale() != Vector2f(-1.f, 1.f)) {
                currentFrame = 0; //reset l'animation
            }
            frames = runningFrames;
            totalFrames = 4;
            //if (!attacking) getSprite().setScale(-1.f, 1.f);
        }
        else {
            frames = standingFrames;
            totalFrames = 3;
        }
        if (!onGround || dashing || dashMomentum) {
            frames[0] = runningFrames[0];
            frames.erase(frames.begin() + 1, frames.end());
            totalFrames = 1;
        }
        if (velocity.y < 0) {
            frames = jumpingFrames;
            totalFrames = 1;
            if (jumped) {
                frames.erase(frames.begin() + 1, frames.end());
                jumped = false;
            }
            else {
                frames.erase(frames.begin(), frames.begin() + 1);
            }
        }
        if (velocity.y > 0) {
            frames = fallingFrames;
            totalFrames = 2;
        }
        if (!attacking) {
            currentFrame = (currentFrame + 1) % totalFrames;
            getSprite().setTextureRect(frames[currentFrame]);
            getSprite().setOrigin(getSprite().getTextureRect().getSize().x / 2, getSprite().getTextureRect().getSize().y / 2);
        }
    }
}