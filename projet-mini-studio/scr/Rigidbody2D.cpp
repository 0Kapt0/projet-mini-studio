#include "../include/Rigidbody2D.hpp"
#include <cmath>
#include <iostream>
#define TILE_SIZE 64

Rigidbody2D::Rigidbody2D(float mass, float drag, Map& map, Entity& entity)
    : mass(mass), drag(drag), gravity(9.8f), map(map), entity(entity), position(0.0f, 0.0f) {
    velocity = Vector2f(0.0f, 0.0f);
    acceleration = Vector2f(0.0f, 0.0f);
}

void Rigidbody2D::applyForce(const Vector2f& force) {
    acceleration += force / mass;
}

void Rigidbody2D::update(float dt) {
    applyGravity(dt);

    velocity += acceleration * dt;

    velocity *= exp(-drag * dt);

    
    if (isOnGround()) {
        if (std::abs(velocity.x) > 10.0f && !moving) {  // 🔥 Applique la friction SEULEMENT si le joueur ne bouge pas
            float friction = 2200.0f * dt;
            if (velocity.x > 0) {
                velocity.x -= friction;
                if (velocity.x < 0) velocity.x = 0;
            }
            else {
                velocity.x += friction;
                if (velocity.x > 0) velocity.x = 0;
            }
        }
    }



    Vector2f newPosition = position + velocity * dt;

    if (!handleCollisions(newPosition)) {
        position = newPosition;
    }

    acceleration = Vector2f(0.0f, 0.0f);
}

void Rigidbody2D::applyGravity(float dt) {
    float gravityForce = 2500.0f;
    float maxFallSpeed = 1200.0f;

    velocity.y += gravityForce * dt;

    if (velocity.y > maxFallSpeed) {
        velocity.y = maxFallSpeed;
    }
    if (isOnGround()) {
        velocity.y = 0;
    }
}



bool Rigidbody2D::handleCollisions(Vector2f& newPosition) {
    bool onGround = false;

    FloatRect bounds = entity.getSprite().getGlobalBounds();
    bounds.left = newPosition.x;
    bounds.top = newPosition.y;

    int left = static_cast<int>(bounds.left + 3);
    int right = static_cast<int>(bounds.left + bounds.width - 3);
    int top = static_cast<int>(bounds.top + 3);
    int bottom = static_cast<int>(bounds.top + bounds.height - 1);

    int belowLeft = static_cast<int>(bounds.left + 3);
    int belowRight = static_cast<int>(bounds.left + bounds.width - 3);
    int belowBottom = static_cast<int>(bounds.top + bounds.height + 2); // 🔥 Vérifie 2px sous le joueur

    bool hasGroundBelowLeft = map.isColliding(belowLeft, belowBottom);
    bool hasGroundBelowRight = map.isColliding(belowRight, belowBottom);

    // ✅ Vérification et correction des collisions au sol
    if (map.isColliding(left, bottom) || map.isColliding(right, bottom)) {
        if (velocity.y > 0) {
            velocity.y = 0;
            newPosition.y = (bottom / TILE_SIZE) * TILE_SIZE - bounds.height + 1;
            onGround = true;
        }
    }
    else if (map.isColliding(left, top) || map.isColliding(right, top)) {
        if (velocity.y < 0) {
            velocity.y = 0;
            newPosition.y = ((top / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
        }
    }

    // ✅ Correction des collisions latérales uniquement si le joueur ne peut pas tomber
    if ((map.isColliding(left, top) || map.isColliding(left, bottom)) && hasGroundBelowLeft) {
        if (velocity.x < 0) {
            velocity.x = 0;
            newPosition.x = (left / TILE_SIZE) * TILE_SIZE + TILE_SIZE + 2; // 🔥 Décalage ajusté
        }
    }
    if ((map.isColliding(right, top) || map.isColliding(right, bottom)) && hasGroundBelowRight) {
        if (velocity.x > 0) {
            velocity.x = 0;
            newPosition.x = (right / TILE_SIZE) * TILE_SIZE - bounds.width - 2; // 🔥 Décalage ajusté
        }
    }

    if (onGround) {
        velocity.y = 0.1f;
    }

    return onGround;
}








void Rigidbody2D::setVelocity(const Vector2f& velocity) {
    this->velocity = velocity;
}

Vector2f Rigidbody2D::getVelocity() const {
    return velocity;
}

void Rigidbody2D::setPosition(const Vector2f& position) {
    this->position = position;
}

Vector2f Rigidbody2D::getPosition() const {
    return position;
}

void Rigidbody2D::setGravity(float gravity) {
    this->gravity = gravity;
}

void Rigidbody2D::setMass(float mass) {
    this->mass = mass;
}

bool Rigidbody2D::isOnGround() const {
    return map.isColliding(position.x - 10, position.y + 50) || 
           map.isColliding(position.x + 10, position.y + 50);
}

void Rigidbody2D::setMoving(bool isMoving) {
    moving = isMoving;
}


