#include "../include/Rigidbody2D.hpp"
#include <cmath>
#include <iostream>

Rigidbody2D::Rigidbody2D(float mass, float drag, Map& map)
    : mass(mass), drag(drag), gravity(9.8f), map(map), position(0.0f, 0.0f) {
    velocity = sf::Vector2f(0.0f, 0.0f);
    acceleration = sf::Vector2f(0.0f, 0.0f);
}

void Rigidbody2D::applyForce(const sf::Vector2f& force) {
    acceleration += force / mass; // F = ma, donc a = F/m
}

void Rigidbody2D::update(float dt) {
    applyGravity(dt); // Applique la gravité à chaque mise à jour

    // Mise à jour de la vélocité
    velocity += acceleration * dt;

    // Appliquer le drag exponentiellement pour éviter la disparition instantanée de la vitesse
    velocity *= std::exp(-drag * dt);

    // Mise à jour de la position selon la vitesse
    sf::Vector2f newPosition = position + velocity * dt;

    // Gestion des collisions avant de valider la nouvelle position
    if (!handleCollisions(newPosition)) {
        position = newPosition;
    }

    // Réinitialiser l'accélération après chaque mise à jour
    acceleration = sf::Vector2f(0.0f, 0.0f);
}

void Rigidbody2D::applyGravity(float dt) {
    float maxFallSpeed = 600.0f; // Vitesse terminale
    sf::Vector2f gravityForce(0.0f, gravity * mass);

    applyForce(gravityForce);

    // Empêcher la vitesse d'excéder la vitesse terminale
    if (velocity.y > maxFallSpeed) {
        velocity.y = maxFallSpeed;
    }
}

bool Rigidbody2D::handleCollisions(sf::Vector2f& newPosition) {
    bool collided = false;

    // Vérification des collisions horizontales
    if (map.isColliding(newPosition.x, position.y)) {
        velocity.x = 0;
        collided = true;
    }

    // Vérification des collisions verticales
    if (map.isColliding(position.x, newPosition.y)) {
        velocity.y = 0;
        collided = true;
    }

    return collided;
}

void Rigidbody2D::setVelocity(const sf::Vector2f& velocity) {
    this->velocity = velocity;
}

sf::Vector2f Rigidbody2D::getVelocity() const {
    return velocity;
}

void Rigidbody2D::setPosition(const sf::Vector2f& position) {
    this->position = position;
}

sf::Vector2f Rigidbody2D::getPosition() const {
    return position;
}

void Rigidbody2D::setGravity(float gravity) {
    this->gravity = gravity;
}

void Rigidbody2D::setMass(float mass) {
    this->mass = mass;
}
