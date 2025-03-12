#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Rigidbody2D.hpp"
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
    void checkGrounded();

private:
    Rigidbody2D rigidbody;
    View playerView;
    int groundLock = 0;

	bool leftButtonHold = false;
    Map& map;
    float speed;
    Vector2f velocity;
    bool canJump = true;
    int jumpNum = 0;
    //DASH
    bool canDash = true;
    bool dashing = false;
    Vector2f dashDirection;
    char lastInputDirection = 'N';
    float dashDuration = 0;
    float dashCooldown = 0.8;
    float dashTimer = 0;
    bool dashMomentum = false;
    //
    Grapple grapple;
    bool grapplingTouched = false;
    //ATTACK
    bool canAttack = true;
    bool attacking = false;
    Texture attackTexture;
    Sprite attackSprite;
    std::string attackDirection = "mid";
    float attackDuration = 0;
    float attackCooldown = 1;
    float attackTimer = 0;

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