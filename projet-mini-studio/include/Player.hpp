﻿#ifndef PLAYER_HPP
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
	void isSwingColliding(Vector2f& newPos, float dt);

private:
    //fonction du update
    void handleGrapplePull(float dt);
    void handleNormalMovement(float dt);
    void handleDashingMovement(float dt);
    void handleMovement(float dt);
    void handleCollisions(float dt);
    void applyMovement(float dt);
    void updateGrapplePosition();
    /*void updateCamera();*/

    //Rigidbody2D rigidbody;
    View playerView;
    Map& map;
    float speed;
    Vector2f velocity;
    int jumpNum = 0;

    // ouvement
    bool canJump = true;
    bool dashMomentum = false;

    //Dash
    bool canDash = true;
    bool dashing = false;
    Vector2f dashDirection;
    char lastInputDirection = 'N';
    float dashDuration = 0;
    float dashCooldown = 0.8;
    float dashTimer = 0;

    //Grapple
    Grapple grapple;
    bool grapplingTouched = false;
    bool leftButtonHold = false;
    bool grappleMove = false;
    bool grappleStuck = false;
    float grappleLength = 0.0f;
    float swingForce = 20.0f;
    float angularVelocity = 0.0f;
    float angle = 0.0f;
    float DAMPING = 0.99f;
    float swingAcceleration = 50.0f;

    //Attaque
    bool canAttack = true;
    bool attacking = false;
    Texture attackTexture;
    Sprite attackSprite;
    string attackDirection = "mid";
    float attackDuration = 0;
    float attackCooldown = 1;
    float attackTimer = 0;
	bool onGround = true;
};

#endif
