#ifndef ENEMYFLYING_HPP
#define ENEMYFLYING_HPP

#include "Entity.hpp"
#include "Player.hpp" 

class EnemyFlying : public Entity {
protected:
    float speed;
    Vector2f velocity;
    float detectionRadius; 

public:
    EnemyFlying();
    EnemyFlying(const Vector2f& size, const Color& color);
    virtual ~EnemyFlying();

    virtual void update(float dt, const Player& player);
    bool isPlayerInRadius(const Vector2f& playerPosition);
    void drawDetectionRadius(sf::RenderWindow& window); 
};

#endif