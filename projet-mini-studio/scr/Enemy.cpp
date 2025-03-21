#include "../include/Enemy.hpp"

using namespace std;
using namespace sf;

Enemy::Enemy(Map& map, Texture& texture)
   : Entity(texture), map(map)
{
    this->map = map;
	speed = 200;
	velocity = Vector2f(0, 0);
}

Enemy::Enemy(const Vector2f& size, const Color& color, Map& map) : Entity(size, color), map(map)
{
    this->map = map;
	speed = 300;
	velocity = Vector2f(0, 0);
}

Enemy::Enemy(Texture& texture, Map& map)
    : Entity(texture), map(map)
{
    this->map = map;
    speed = 200;
    velocity = Vector2f(0, 0);
}

Enemy::~Enemy()
{
}

void Enemy::update(float dt, Player& player, RenderWindow& window) {
	velocity.y += 14.8f;
   
	Vector2f position = getSprite().getPosition();
	FloatRect bounds = getSprite().getGlobalBounds();

	float screenWidth = 800;
	float screenHeight = 600;


	if (position.x <= 0 && velocity.x <= 0) {
		velocity.x = speed; 
	}
	else if (position.x + bounds.width >= screenWidth && velocity.x >= 0) {
		velocity.x = -speed; 
	}

	if (getSprite().getPosition().y > 900)
	{
		velocity.y = 0;
	}
    isColliding(getSpriteConst().getPosition().x, getSpriteConst().getPosition().y, dt);

    getSprite().move(velocity * dt);

}

void Enemy::isColliding(int x, int y, float dt)
{
    int newX = getSpriteConst().getPosition().x + velocity.x * dt;
    int newY = getSpriteConst().getPosition().y + velocity.y * dt;

    collided = false;

    // Vérifie la collision avant d'appliquer le mouvement
    if (map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX + getSpriteConst().getGlobalBounds().width, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
    {
        newX = (static_cast<int>((getSprite().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width) / 64)) * 64 + 64 - getTexture().getSize().x / 2;
        getSprite().setPosition(newX - 0.1, getSpriteConst().getPosition().y);
        velocity.x = 0;
        collided = true;
    }

    else if (velocity.x < 0)
    {
        if (map.isColliding(newX, getSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getSpriteConst().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height / 2))
        {
            newX = (static_cast<int>(getSprite().getGlobalBounds().left / 64)) * 64 + getTexture().getSize().x / 2;
            getSprite().setPosition(newX + 0.1, getSpriteConst().getPosition().y);
            velocity.x = 0;
            collided = true;
        }
    }
    if (velocity.y > 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY + getSpriteConst().getGlobalBounds().height) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY + getSpriteConst().getGlobalBounds().height))
        {
            newY = (static_cast<int>((getSprite().getGlobalBounds().top + getSpriteConst().getGlobalBounds().height) / 64)) * 64 + 64 - getTexture().getSize().y / 2;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY - 0.1);
            velocity.y = 0;
            collided = true;
        }
    }
    else if (velocity.y < 0)
    {
        if (map.isColliding(getSpriteConst().getGlobalBounds().left, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width, newY) || map.isColliding(getSpriteConst().getGlobalBounds().left + getSpriteConst().getGlobalBounds().width / 2, newY))
        {
            newY = (static_cast<int>(getSprite().getGlobalBounds().top / 64)) * 64 + getTexture().getSize().y / 2;
            getSprite().setPosition(getSpriteConst().getPosition().x, newY + 0.1);
            velocity.y = 0;
            collided = true;
        }
    }
}

void Enemy::applySmoothPushback(float deltaTime, Player& player) {
    if (!isPushingBack) return;

    float elapsed = pushbackClock.getElapsedTime().asSeconds();
    float pushDuration = 0.5f;
    float factor = 1.0f - (elapsed / pushDuration);

    if (factor <= 0) {
        isPushingBack = false;
        return;
    }

    Vector2f pushStep = pushbackDirection * (pushbackStrength * factor * deltaTime);
    getSprite().setPosition(getSprite().getPosition() - pushStep);
}

void Enemy::pushBack(Player& player) {
    Vector2f bossPos = getSprite().getPosition();
    Vector2f playerPos = player.getSprite().getPosition();
    Vector2f offSet = { 0, 0 };
    bossPos = bossPos + offSet;
    pushbackDirection = playerPos - bossPos;

    float length = sqrt(pushbackDirection.x * pushbackDirection.x + pushbackDirection.y * pushbackDirection.y);
    if (length != 0) {
        pushbackDirection /= length;
    }

    pushbackStrength = 500.0f;
    isPushingBack = true;
    pushbackClock.restart();
}

void Enemy::setTexture(Texture& tex, int frameWidth, int frameHeight, int _totalFrames, float frameDuration) {
    getSprite().setTexture(tex);
    getSprite().setTextureRect(IntRect(0, 0, frameWidth, frameHeight));

    frames.clear();
    for (int i = 0; i < _totalFrames; ++i) {
        frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
    }

    totalFrames = _totalFrames;
    frameTime = frameDuration;
    getSprite().setTextureRect(frames[currentFrame]);
    getSprite().setOrigin(frameWidth / 2, frameHeight / 2);
}
void Enemy::animate(float deltaTime) {

}