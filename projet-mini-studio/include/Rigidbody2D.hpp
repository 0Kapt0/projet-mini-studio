#ifndef RIGIDBODY2D_HPP
#define RIGIDBODY2D_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Rigidbody2D {
public:
    // Constructeur prenant en compte la masse, la r�sistance et la carte
    Rigidbody2D(float mass, float drag, Map& map);

    // Appliquer une force externe
    void applyForce(const sf::Vector2f& force);

    // Mise � jour physique
    void update(float dt);

    // G�rer la gravit�
    void applyGravity(float dt);

    // G�rer les collisions avec la carte
    bool handleCollisions(sf::Vector2f& newPosition);

    // Getter et Setter pour la v�locit�
    void setVelocity(const sf::Vector2f& velocity);
    sf::Vector2f getVelocity() const;

    // Getter et Setter pour la position
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    // Getter et Setter pour la gravit� et la masse
    void setGravity(float gravity);
    void setMass(float mass);

private:
    float mass;                // Masse du corps rigide
    float drag;                // Coefficient de r�sistance
    float gravity;             // Force de gravit�
    sf::Vector2f velocity;     // V�locit� de l'objet
    sf::Vector2f acceleration; // Acc�l�ration actuelle
    sf::Vector2f position;     // Position actuelle de l'objet
    Map& map;                  // R�f�rence vers la carte pour la d�tection des collisions
};

#endif // RIGIDBODY2D_HPP
