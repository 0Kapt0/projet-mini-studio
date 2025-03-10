#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "Grapple.hpp"
#include "Entity.hpp"

using namespace sf;

class Player : public Entity {
public:
    Player();
    Player(const Vector2f& size, const Color& color);
    ~Player();

    void update(float dt);
    void draw(RenderWindow& window);
    void handleInput(const Event& event, const RenderWindow& window);

private:
    float speed;
    Vector2f velocity;
    bool canJump = true;
    int jumpNum = 0;
    Grapple grapple;
    bool grapplingTouched = false;
};

#endif // PLAYER_HPP

