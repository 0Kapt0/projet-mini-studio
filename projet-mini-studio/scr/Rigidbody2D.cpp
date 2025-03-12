#include "../include/Rigidbody2D.hpp"
#include <cmath>
#include <iostream>

Rigidbody2D::Rigidbody2D(float mass, float drag)
    : mass(mass), drag(drag), gravity(9.8f), map(map) {
    velocity = sf::Vector2f(0.0f, 0.0f);
    acceleration = sf::Vector2f(0.0f, 0.0f);
}

void Rigidbody2D::applyForce(const sf::Vector2f& force) {
    acceleration += force / mass; // F = ma, donc a = F/m
}

void Rigidbody2D::update(float dt) {
    applyGravity(dt); // Applique la gravité à chaque mise à jour

    // Appliquer l'accélération
    velocity += acceleration * dt;

    // Appliquer la résistance (drag) sur la vitesse
    velocity *= drag;

    // Mise à jour de la position selon la vitesse
    position += velocity * dt;

    // Réinitialiser l'accélération après chaque mise à jour
    acceleration = sf::Vector2f(0.0f, 0.0f);
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

void Rigidbody2D::applyGravity(float dt) {
    // Appliquer la gravité sur l'axe Y
    applyForce(sf::Vector2f(0.0f, gravity * mass));
}

void Rigidbody2D::handleCollisions(float dt) {
    if (std::isnan(position.x) || std::isnan(position.y)) {
        std::cerr << "Position invalide!" << std::endl;
        return;  // Sortir si la position est invalide
    }
    if (std::isnan(velocity.x) || std::isnan(velocity.y)) {
        std::cerr << "Vitesse invalide!" << std::endl;
        return;  // Sortir si la vitesse est invalide
    }

    // Vérification des collisions ici
    if (map.isColliding(position.x + velocity.x * dt, position.y)) {
        velocity.x = 0;
    }
    if (map.isColliding(position.x, position.y + velocity.y * dt)) {
        velocity.y = 0;
    }
}

