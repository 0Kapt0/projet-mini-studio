#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Rigidbody2D.hpp"
#include "Grapple.hpp"
#include "Entity.hpp"
#include "../include/SoundManager.hpp"

using namespace sf;

class Player : public Entity {
public:
    Clock jumpCooldownClock;
	//SoundManager soundManager;
    //Player(Map& map);
    Player(Texture& tex, Map& map);
    Player(const Vector2f& size, const Color& color, Map& map);
    ~Player();
    void update(float dt);
    void draw(RenderWindow& window);
    void drawHearts(RenderWindow& window);
    void handleInput(const Event& event, RenderWindow& window, float dt);
    void isSwingColliding(Vector2f& newPos, float dt);
    Sprite getAttackHitBox();
    bool isAttacking();
    int getMaxHp();
    void setMaxHp(int newMaxHp);
    void oneUp(int value);

    int hpCeiling = 3;
    int killCount = 0;
    bool doubleJumpUnlocked = true;
    bool dashUnlocked = true;
    bool grappleUnlocked = true;

    void setTexture(Texture& tex, int frameWidth, int frameHeight, int totalFrames, float frameDuration) override;
    int frameWidthResize = 0;
    int frameHeightResize = 0;
    int frameStartingX = 0;
    int frameStartingY = 0;
    void animate(float deltaTime) override;
    std::vector<IntRect> standingFrames;
    std::vector<IntRect> runningFrames;
    std::vector<IntRect> jumpingFrames;
    bool jumped = false;
    std::vector<IntRect> attackingFrames;
    std::vector<IntRect> attackHitboxFrames;
    std::vector<IntRect> fallingFrames;

    void attackFrameSwap();
    IntRect formerFrame0;

    CircleShape hurtbox;

private:
    //fonction du update
    void handleGrapplePull(float dt);
    void handleNormalMovement(float dt);
    void handleDashingMovement(float dt);
    void handleMovement(float dt);
    void handleCollisions(float dt);
    void handleAttack(float dt);
    void applyMovement(float dt);
    void updateGrapplePosition();
    void updateCamera();

    void handleBoundingBoxCollision(float dt);
    void handleDashingCollision(float dt);
    bool handleSlopeCollisionOnce();
    void isColliding(int x, int y, float dt);

    View playerView;
    Map& map;
    float speed;
    Vector2f velocity;
    int jumpNum = 0;

    //Health
    Texture heartTexure;
    Texture heartemptyTexure;
    Sprite heart1;
    Sprite heartempty;
    int maxHp = 3;

    //Mouvement
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
    bool attackHitboxActive = false;
    Sprite attackSprite;
    string attackDirection = "right";
    float attackDuration = 0;
    float attackCooldown = 0;
    float attackTimer = 0;
    bool onGround = true;
    Vector2f lastAttackPosition;
};

#endif
