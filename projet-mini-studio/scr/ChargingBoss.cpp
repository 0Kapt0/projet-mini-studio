#include "../include/ChargingBoss.hpp"

ChargingBoss::ChargingBoss(Map& map) : Enemy(map) {

}

ChargingBoss::ChargingBoss(const Vector2f& size, const Color& color, Map& map) : Enemy(size, color, map) {
	getSprite().setPosition(500, 780);
}

void ChargingBoss::update(float dt) {

}

void ChargingBoss::behavior(float dt, Player& player, RenderWindow& window) {
	target = player.getSprite().getPosition();
	distancePlayer = std::sqrt(pow(target.x - getSprite().getPosition().x, 2) + pow(target.y - getSprite().getPosition().y, 2));
	switch (currentState) {
	case CHASING :
		//std::cout << "chasing\n";
		chase(player, dt);
		if (distancePlayer < window.getSize().x * 0.3f && player.getSprite().getPosition().y + player.getHeight() > getSprite().getPosition().y) {
			currentState = CHARGING;
			dashTiming = dashTimingVector[rand() % dashTimingVector.size()];
		}
		break;
	case CHARGING :
		//std::cout << "CHARGING\n";
		charge(dt);
		break;

	case DASHING :
		//std::cout << "DAAAASHING\n";
		dash(dt);
		break;

	case STUNNED :
		//std::cout << "stunned\n";
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
	chargingTimer += dt;
	if (chargingTimer < dashTiming) {
		velocity.x = -velocity.x;
	}
	else {
		velocity.x = 0;
		getSprite().setColor(Color::Red);
	}
	getSprite().move(velocity * dt);
	if (chargingTimer > dashTiming + 0.25f) {
		getSprite().setColor(Color::Green);
		chargingTimer = 0;
		currentState = DASHING;
	}
}

void ChargingBoss::dash(float dt) {
	if (dashDirection == "left") {
		velocity.x = -speed * 5.f;
	}
	if (dashDirection == "right") {
		velocity.x = speed * 5.f;
	}
	//getSprite().move(velocity * dt);
	dashTimer += dt;
	if (dashTimer > 0.5f) {
		velocity.x = 0;
		getSprite().setColor(Color::Blue);
	}
	if (dashTimer > 1.f) {
		dashTimer = 0;
		currentState = CHASING;
	}
	isColliding(dt);
	if (collided) {
		currentState = STUNNED;
		dashTimer = 0;

		if (dashDirection == "left") {
			velocity.x = speed;
		}
		if (dashDirection == "right") {
			velocity.x = -speed;
		}
		velocity.y = speed;
	}
	getSprite().move(velocity* dt);
}

void ChargingBoss::stunned(float dt) {
	//ANIMATIONS
	stunTimer += dt;
	if (velocity.x > 0) {
		velocity.x -= speed * 0.03f;
	}
	else {
		velocity.x = 0;
	}
	velocity.y = -velocity.y; //ANIMATIONS
	if (stunTimer > 3.f) {
		stunTimer = 0;
		velocity.y = 0;
		currentState = CHASING;
	}
	getSprite().move(velocity* dt);
}

//void ChargingBoss::draw(RenderWindow& window) {
//
//}