#ifndef RIGIDBODY2D_HPP
#define RIGIDBODY2D_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Rigidbody2D {
public:
    // Constructeur prenant en compte la masse, la résistance et la carte
    Rigidbody2D(float mass, float drag, Map& map);

    // Appliquer une force externe
    void applyForce(const sf::Vector2f& force);

    // Mise à jour physique
    void update(float dt);

    // Gérer la gravité
    void applyGravity(float dt);

    // Gérer les collisions avec la carte
    bool handleCollisions(sf::Vector2f& newPosition);

    // Getter et Setter pour la vélocité
    void setVelocity(const sf::Vector2f& velocity);
    sf::Vector2f getVelocity() const;

    // Getter et Setter pour la position
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    // Getter et Setter pour la gravité et la masse
    void setGravity(float gravity);
    void setMass(float mass);

private:
    float mass;                // Masse du corps rigide
    float drag;                // Coefficient de résistance
    float gravity;             // Force de gravité
    sf::Vector2f velocity;     // Vélocité de l'objet
    sf::Vector2f acceleration; // Accélération actuelle
    sf::Vector2f position;     // Position actuelle de l'objet
    Map& map;                  // Référence vers la carte pour la détection des collisions
};

#endif // RIGIDBODY2D_HPP
