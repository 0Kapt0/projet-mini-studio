#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>
#include "Entity.hpp" 
#include "Map.hpp"
#include "Player.hpp"

class Enemy : public Entity {
protected:
    float speed;
    Vector2f velocity;
    Map map;

public:
    Enemy(Map& map, Texture& texture);
    Enemy(const Vector2f& size, const Color& color, Map& map);
    Enemy(Map& map);
    virtual ~Enemy();
    virtual void update(float dt, Player& player, RenderWindow& window);
    virtual void isColliding(int x, int y, float dt);
    void applySmoothPushback(float deltaTime, Player& player);
    void pushBack(Player& player);
    bool isPushingBack = false;
    Clock pushbackClock;
    Vector2f pushbackDirection;
    float pushbackStrength;
    CircleShape hitbox;


    void setTexture(Texture& tex, int frameWidth, int frameHeight, int totalFrames, float frameDuration) override;
    void animate(float deltaTime) override;
};

#endif

//here is my Enemy Flying, please add a fsm AI that has 4 states : patrolling (betwens set points),  chasing (following the player when they enter the detection radius), searching (going to the last known player position after he leaves the detection radius) and attacking (when the enemy touches the player