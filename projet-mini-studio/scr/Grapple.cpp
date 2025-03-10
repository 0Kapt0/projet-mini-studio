// Grapple.cpp
#include "../include/Grapple.hpp"

Grapple::Grapple(float maxRange) : maxRange(maxRange), active(false) {
    line.setPrimitiveType(Lines);
    line.resize(2);
}

void Grapple::launch(const Vector2f& startPosition, const Vector2f& direction) {
    this->startPosition = startPosition;
    this->direction = direction;
    line[0].position = startPosition;
    line[1].position = startPosition;
    active = true;
}

void Grapple::update(float deltaTime) {
    if (!active) return;

    Vector2f newPosition = line[1].position + direction * deltaTime * 800.0f; // Vitesse du grappin
    if (std::hypot(newPosition.x - startPosition.x, newPosition.y - startPosition.y) > maxRange) {
        active = false;
    }
    else {
        line[1].position = newPosition;
    }
}

void Grapple::draw(RenderWindow& window) {
    if (active) {
        window.draw(line);
    }
}

bool Grapple::isActive() const {
    return active;
}

Vector2f Grapple::getDirection() const {
    return direction;
}

void Grapple::updateStartPosition(const Vector2f& newPosition) {
    startPosition = newPosition;
    line[0].position = newPosition;
}