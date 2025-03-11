#include "../include/ChargingBoss.hpp"

ChargingBoss::ChargingBoss(Map& map) : Enemy(map) {

}

ChargingBoss::ChargingBoss(const Vector2f& size, const Color& color, Map& map) : Enemy(size, color, map) {
	getSprite().setPosition(500, 800);
}

void ChargingBoss::update(float dt) {

}

void ChargingBoss::behavior(float dt, Player& player, RenderWindow& window) {
	target = player.getSprite().getPosition();
	distancePlayer = std::sqrt(pow(target.x - getSprite().getPosition().x, 2) + pow(target.y - getSprite().getPosition().y, 2));
	std::cout << currentState << std::endl;
	switch (currentState) {
	case CHASING :
		std::cout << "chasing\n";
		chase(player, dt);
		if (distancePlayer < window.getSize().x * 0.3f) {
			currentState = CHARGING;
		}
		break;
	case CHARGING :
		std::cout << "CHARGING\n";
		charge(dt);
		break;

	case DASHING :
		std::cout << "DAAAASHING\n";
		dash(dt);
		break;

	case STUNNED :
		std::cout << "stunned\n";
		stunned(dt);
		break;
		
	}
}

void ChargingBoss::chase(Player& player, float dt) {
	if (target.x < getSprite().getPosition().x) {
		velocity.x = -speed;
		dashDirection = "left";
	}
	else { 
		velocity.x = speed;
		dashDirection = "right";
	}
	getSprite().move(velocity * dt);
}

void ChargingBoss::charge(float dt) {
	//animation de chargement de l'attaque
	velocity.x = -velocity.x;
	getSprite().move(velocity * dt);
	chargingTimer += dt;
	if (chargingTimer > 3.f) {
		chargingTimer = 0;
		currentState = DASHING;
	}
}

void ChargingBoss::dash(float dt) {
	if (dashDirection == "left") {
		velocity.x = -speed * 3.f;
	}
	if (dashDirection == "right") {
		velocity.x = speed * 3.f;
	}
	getSprite().move(velocity * dt);
	dashTimer += dt;
	if (dashTimer > 1.f) {
		dashTimer = 0;
		currentState = CHASING;
	}
	
	if (isColliding(dt)) {
		currentState = STUNNED;
		velocity.x = 0;
		velocity.y = speed;
	}
}

void ChargingBoss::stunned(float dt) {
	//ANIMATIONS
	stunTimer += dt;
	velocity.y = -velocity.y;
	if (stunTimer > 3.f) {
		stunTimer = 0;
		velocity.y = 0;
		currentState = CHASING;
	}
}

//void ChargingBoss::draw(RenderWindow& window) {
//
//}