#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Grapple.hpp"
#include "Entity.hpp"

using namespace sf;

class Player : public Entity {
public:
    Player();
    Player(const Vector2f& size, const Color& color);
    ~Player();

    void update(float dt);
    void draw(RenderWindow& window);
    void handleInput(const Event& event, const RenderWindow& window);

private:
	float speed;
	Vector2f velocity;
	bool canJump = true;
	int jumpNum = 0;
	bool canDash = true;
	bool dashing = false;
	Vector2f dashDirection;
	char lastInputDirection = 'N';
	float dashDuration = 0;
	float dashCooldown = 0.8;
	float dashTimer = 0;
	Grapple grapple;
	bool grapplingTouched = false;
public:
	Player();
	Player(const Vector2f& size, const Color& color);
	~Player();
	void update(float dt);
};