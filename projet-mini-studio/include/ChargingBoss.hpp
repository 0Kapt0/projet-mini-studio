#ifndef CHARGINGBOSS_HPP
#define CHARGINGBOSS_HPP

#include "../include/Enemy.hpp"
#include "../include/Player.hpp"

class ChargingBoss : public Enemy {
public:
	Vector2f target;
	float distancePlayer = 0;
	std::string dashDirection = "nothing";
	enum State { CHASING, CHARGING, DASHING, STUNNED };
	State currentState = CHASING;

	float chargingTimer = 0;
	float dashTimer = 0;
	float stunTimer = 0;

	std::vector<float> dashTimingVector = { 0.5, 0.75, 1, 1.25, 1.5, 1.25, 1.5 };
	float dashTiming = 0;

	ChargingBoss(Map& map);
	ChargingBoss(const Vector2f& size, const Color& color, Map& map);

	//void draw(RenderWindow& window) override;

	void update(float dt) override;
	void behavior(float dt, Player& player, RenderWindow& window);
	void chase(Player& player, float dt);
	void charge(float dt);
	void dash(float dt);
	void stunned(float dt);
};

#endif
