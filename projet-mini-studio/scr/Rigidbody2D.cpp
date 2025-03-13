#include "../include/Rigidbody2D.hpp"
#include <cmath>
#include <iostream>
#define TILE_SIZE 64

Rigidbody2D::Rigidbody2D(float mass, float drag, Map& map)
    : mass(mass), drag(drag), gravity(9.8f), map(map), position(0.0f, 0.0f) {
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
        velocity.x *= 0.92f;
    }

    if (abs(velocity.x) < 5.0f) {
        velocity.x = 0;
    }

    Vector2f newPosition = position + velocity * dt;

    if (!handleCollisions(newPosition)) {
        position = newPosition;
    }

    acceleration = Vector2f(0.0f, 0.0f);
}

void Rigidbody2D::applyGravity(float dt) {
    float gravityForce = 1500.0f;
    float maxFallSpeed = 800.0f;

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

    FloatRect bounds(position.x, position.y, 50, 50);
    Vector2f spriteOrigin(bounds.width / 2, bounds.height / 2);
    FloatRect correctedBounds(newPosition.x - (bounds.width / 2), newPosition.y - (bounds.height / 2), bounds.width, bounds.height);

    int left = correctedBounds.left + 5;
    int right = correctedBounds.left + correctedBounds.width - 5;
    int top = correctedBounds.top + 3;
    int bottom = correctedBounds.top + correctedBounds.height - 1;


    cout << "[DEBUG] Collision Gauche: " << map.isColliding(left, top) << " " << map.isColliding(left, bottom) << endl;
    cout << "[DEBUG] Collision Droite: " << map.isColliding(right, top) << " " << map.isColliding(right, bottom) << endl;


    if (map.isColliding(left, bottom) || map.isColliding(right, bottom)) {
        if (velocity.y > 0) {
            velocity.y = 0;
            newPosition.y = (bottom / TILE_SIZE) * TILE_SIZE - correctedBounds.height + 1;
            onGround = true;
            cout << "[DEBUG] ✅ Joueur AU SOL, Position Y corrigée: " << newPosition.y << endl;
        }
    }
    else if (map.isColliding(left, top) || map.isColliding(right, top)) {
        if (velocity.y < 0) {
            velocity.y = 0;
            newPosition.y = ((top / TILE_SIZE) * TILE_SIZE) + TILE_SIZE;
            cout << "[DEBUG] ⛔ Joueur HEURTE un plafond, Y corrigé: " << newPosition.y << endl;
        }
    }

    if (map.isColliding(left, top) || map.isColliding(left, bottom)) {
        if (velocity.x < 0) {
            velocity.x *= -0.2f;
        }
    }
    if (map.isColliding(right, top) || map.isColliding(right, bottom)) {
        if (velocity.x > 0) {
            velocity.x *= -0.2f;
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

