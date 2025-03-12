#ifndef RIGIDBODY2D_HPP
#define RIGIDBODY2D_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Rigidbody2D {
public:
    Rigidbody2D(float mass, float drag = 0.98f); // Masse et résistance à l'air (drag)

    void applyForce(const sf::Vector2f& force); // Appliquer une force (par exemple, gravité, saut)
    void update(float dt); // Met à jour la position, la vitesse et applique la gravité
    void setVelocity(const sf::Vector2f& velocity); // Définit directement la vitesse
    sf::Vector2f getVelocity() const; // Retourne la vitesse actuelle

    void setPosition(const sf::Vector2f& position); // Définit la position du joueur
    sf::Vector2f getPosition() const; // Retourne la position actuelle

    void setGravity(float gravity); // Définit la gravité à appliquer
    void setMass(float mass); // Définit la masse du corps

    void handleCollisions(float dt); // Gestion des collisions avec la carte

private:
    Map& map;

    sf::Vector2f position; // Position du corps
    sf::Vector2f velocity; // Vitesse actuelle
    sf::Vector2f acceleration; // Accélération actuelle

    float mass; // Masse du corps
    float gravity; // Gravité appliquée
    float drag; // Résistance à l'air (drag)

    void applyGravity(float dt); // Applique la gravité
};

#endif // RIGIDBODY2D_HPP
