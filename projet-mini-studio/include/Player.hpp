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
    void draw(RenderWindow& window) override;
    void handleInput(const Event& event, const RenderWindow& window);

private:
    bool canJump = true;
    int jumpNum = 0;
    //DASH
    bool canDash = true;
    bool dashing = false;
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

};

#endif