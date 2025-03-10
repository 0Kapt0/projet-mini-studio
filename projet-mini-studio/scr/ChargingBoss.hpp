#ifndef CHARGINGBOSS_HPP
#define CHARGINGBOSS_HPP

#include "../include/Enemy.hpp"

class ChargingBoss : public Enemy {
public:
	ChargingBoss();
	ChargingBoss(const Vector2f& size, const Color& color);

	void behavior();
};

#endif
