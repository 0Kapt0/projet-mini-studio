#ifndef RIGIDBODY2D_HPP
#define RIGIDBODY2D_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"
#include "Entity.hpp"

class Rigidbody2D {
public:

    Rigidbody2D(float mass, float drag, Map& map, Entity& entity);

    void applyForce(const Vector2f& force);
    void update(float dt);
    void applyGravity(float dt);
    bool handleCollisions(Vector2f& newPosition);
    void setVelocity(const Vector2f& velocity);
    Vector2f getVelocity() const;
    void setPosition(const Vector2f& position);
    Vector2f getPosition() const;
    void setGravity(float gravity);
    void setMass(float mass);
    bool isOnGround() const;
    void setMoving(bool isMoving);

private:
    bool moving = false;
    float mass;
    float drag;
    float gravity;
    Vector2f velocity;
    Vector2f acceleration;
    Vector2f position;
    Map& map;
    Entity& entity;
};

#endif
