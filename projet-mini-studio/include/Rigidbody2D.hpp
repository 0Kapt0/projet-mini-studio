#ifndef RIGIDBODY2D_HPP
#define RIGIDBODY2D_HPP

#include <SFML/Graphics.hpp>
#include "Map.hpp"

class Rigidbody2D {
public:
    Rigidbody2D(float mass, float drag = 0.98f); // Masse et r�sistance � l'air (drag)

    void applyForce(const sf::Vector2f& force); // Appliquer une force (par exemple, gravit�, saut)
    void update(float dt); // Met � jour la position, la vitesse et applique la gravit�
    void setVelocity(const sf::Vector2f& velocity); // D�finit directement la vitesse
    sf::Vector2f getVelocity() const; // Retourne la vitesse actuelle

    void setPosition(const sf::Vector2f& position); // D�finit la position du joueur
    sf::Vector2f getPosition() const; // Retourne la position actuelle

    void setGravity(float gravity); // D�finit la gravit� � appliquer
    void setMass(float mass); // D�finit la masse du corps

    void handleCollisions(float dt); // Gestion des collisions avec la carte

private:
    Map& map;

    sf::Vector2f position; // Position du corps
    sf::Vector2f velocity; // Vitesse actuelle
    sf::Vector2f acceleration; // Acc�l�ration actuelle

    float mass; // Masse du corps
    float gravity; // Gravit� appliqu�e
    float drag; // R�sistance � l'air (drag)

    void applyGravity(float dt); // Applique la gravit�
};

#endif // RIGIDBODY2D_HPP
