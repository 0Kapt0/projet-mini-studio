// Grapple.hpp
#ifndef GRAPPLE_HPP
#define GRAPPLE_HPP

#include <SFML/Graphics.hpp>
using namespace sf;

class Grapple {
public:
    Grapple(float maxRange);

    void launch(const Vector2f& startPosition, const Vector2f& direction);
    void update(float deltaTime);
    void draw(RenderWindow& window);
    bool isActive() const;
    Vector2f getDirection() const;
    void updateStartPosition(const Vector2f& newPosition);

private:
    VertexArray line;
    Vector2f startPosition;
    Vector2f direction;
    float maxRange;
    bool active;
};

#endif // GRAPPLE_HPP
