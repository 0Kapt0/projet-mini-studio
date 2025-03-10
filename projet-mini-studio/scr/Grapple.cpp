#include "../include/Grapple.hpp"
#include "../include/Map.hpp"
#include <cmath>

Grapple::Grapple(float maxRange, Map& map) : maxRange(maxRange), active(false), stuck(false), map(map) {
    line.setPrimitiveType(Lines);
    line.resize(2);
}

void Grapple::launch(const Vector2f& startPosition, const Vector2f& direction) {
    this->startPosition = startPosition;
    this->direction = direction;
    line[0].position = startPosition;
    line[1].position = startPosition;
    active = true;
    stuck = false; // Réinitialiser l'indicateur lors du lancement
}

void Grapple::update(float deltaTime, RenderWindow& window) {
    if (active && !stuck) {
        Vector2f newPosition = line[1].position + direction * maxRange * deltaTime;

        // Vérifier la collision avec le mur
        if (map.isColliding(newPosition.x, newPosition.y)) {
            stuck = true; // Planter le grappin dans le mur
        }
        else {
            line[1].position = newPosition;
        }
    }

    float distance = std::sqrt(std::pow(line[1].position.x - line[0].position.x, 2) + std::pow(line[1].position.y - line[0].position.y, 2));
    if (distance > maxRange) {
        active = false;
        stuck = false;
        line[1].position = { 0, 0 };
    }

    // Effectuer un raycast pour vérifier les collisions avec d'autres murs
    if (stuck) {
        Vector2f currentPosition = line[0].position;
		direction = line[1].position - line[0].position;
		direction /= distance; // Normaliser la direction
        Vector2f step = direction * 1.0f; // Ajustez la taille du pas si nécessaire

        while (std::sqrt(std::pow(currentPosition.x - line[1].position.x, 2) + std::pow(currentPosition.y - line[1].position.y, 2)) > 5) {
            currentPosition.x += step.x;
            currentPosition.y += step.y;
            if (map.isColliding(currentPosition.x, currentPosition.y)) {
                active = false; // Désactiver le grappin s'il croise un autre mur
				stuck = false;
                line[1].position = { 0, 0 };
				cout << "plus stuck" << endl;
                break;
            }
        }
    }
}

void Grapple::draw(RenderWindow& window) {
    window.draw(point);
    if (active) {
        window.draw(line);
    }
}

bool Grapple::isActive() const {
    return active;
}

bool Grapple::isStuck() const {
    return stuck;
}

Vector2f Grapple::getDirection() const {
    return direction;
}

Vector2f Grapple::getStuckPosition() const {
    return line[1].position;
}

void Grapple::updateStartPosition(const Vector2f& newPosition) {
    startPosition = newPosition;
    line[0].position = newPosition;
}
