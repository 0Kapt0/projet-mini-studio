// Grapple.hpp
#ifndef GRAPPLE_HPP
#define GRAPPLE_HPP
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map.hpp"
using namespace sf;
using namespace std;

class Grapple {
public:
    Grapple(float maxRange, Map& map);

    void launch(const Vector2f& startPosition, const Vector2f& direction);
    void update(float deltaTime, RenderWindow& window);
    void draw(RenderWindow& window);
    bool isActive() const;
    bool isStuck() const;
    Vector2f getDirection() const;
    void updateStartPosition(const Vector2f& newPosition);
    Vector2f getStuckPosition() const;
	void setStuck(bool stuck);
	void setActive(bool active);
	float getGrappleLength() const;
	void setGrappleLength(float grappleLength);

private:
	CircleShape point;
    Map& map;
    VertexArray line;
    Vector2f startPosition;
    Vector2f direction;
    float maxRange;
    bool active;
    bool stuck;
    float grappleLength;
};

#endif // GRAPPLE_HPP
