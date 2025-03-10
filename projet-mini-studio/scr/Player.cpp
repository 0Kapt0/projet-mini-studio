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

void Player::update(float dt)
{
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

    if (Keyboard::isKeyPressed(Keyboard::Q))
        velocity.x = -speed;
    if (Keyboard::isKeyPressed(Keyboard::D))
        velocity.x = speed;
    if (Keyboard::isKeyPressed(Keyboard::Z) && canJump)
    {
        velocity.y = -speed;
        jumpNum++;
        canJump = false;
    }
    if (Keyboard::isKeyPressed(Keyboard::S) && getSprite().getPosition().y < 200)
        velocity.y += speed / 4;

    getSprite().move(velocity * dt);


    if (grapple.isActive()) {
        grapple.updateStartPosition(getSprite().getPosition());
    }
    // Mettre à jour le grappin
    grapple.update(dt);
}

void Player::draw(RenderWindow& window)
{
    // Dessiner le joueur
    window.draw(getSprite());

    // Dessiner le grappin
    grapple.draw(window);
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
