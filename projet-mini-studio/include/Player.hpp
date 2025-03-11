#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Grapple.hpp"
#include "Entity.hpp"

using namespace sf;

class Player : public Entity {
public:
    Player(Map& map);
    Player(const Vector2f& size, const Color& color, Map& map);
    ~Player();

    void update(float dt);
    void draw(RenderWindow& window);
    void handleInput(const Event& event, RenderWindow& window, float dt);
	void isColliding(int x, int y, float dt);

private:
	bool leftButtonHold = false;
    Map& map;
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
	float grappleLength = 0.0f;
	bool grappleMove = false;
	bool onGround = true;
	float swingForce = 20.0f;
	float angularVelocity = 0.0f;
	float angle = 0.0f;
	float DAMPING = 0.99f;
	float swingAcceleration = 50.0f;
	bool grappleStuck = false;
};

#endif