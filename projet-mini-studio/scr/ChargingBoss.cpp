#include "../include/ChargingBoss.hpp"

ChargingBoss::ChargingBoss(Map& map) : Enemy(map) {

	hp = 15;
	type = "ChargingBoss";
	Texture texture;
	texture.create(100, 195);
	Image image;
	image.create(100, 195, Color::Red);
	texture.update(image);
	wallCollideSprite.setTexture(texture);
	wallCollideSprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}

ChargingBoss::ChargingBoss(const Vector2f& size, const Color& color, Map& map) : Enemy(size, color, map) {
	//getSprite().setPosition(500, 800);
	type = "ChargingBoss";
}


void ChargingBoss::update(float dt, Player& player, RenderWindow& window) {
	window.draw(wallCollideSprite);
	invincibilityAfterHit(dt);
	target = player.getSprite().getPosition();
	distancePlayer = std::sqrt(pow(target.x - getSprite().getPosition().x, 2) + pow(target.y - getSprite().getPosition().y, 2));
	if (target.x > getPosX()) {
		wallCollideSprite.setPosition(getPosX() + wallCollideSprite.getGlobalBounds().width / 2, getPosY());
		if (currentState != DASHING && currentState != STUNNED) {
			getSprite().setScale(-1.f, 1.f);
		}
	}
	if (target.x < getPosX()) {
		wallCollideSprite.setPosition(getPosX() - wallCollideSprite.getGlobalBounds().width / 2, getPosY());
		if (currentState != DASHING && currentState != STUNNED) {
			getSprite().setScale(1.f, 1.f);
		}
	}
	getSprite().setOrigin(getSprite().getTextureRect().getSize().x / 2, getSprite().getTextureRect().getSize().y / 2);
	switch (currentState) {
		case CHASING:
			//std::cout << "chasing\n";
			chase(player, dt);
			if (distancePlayer < window.getSize().x * 0.3f && player.getSprite().getPosition().y + player.getHeight() > getSprite().getPosition().y) {
				currentState = CHARGING;
				dashTiming = dashTimingVector[rand() % dashTimingVector.size()];
			}
			break;
		case CHARGING:
			//std::cout << "CHARGING\n";
			charge(dt);
			break;

		case DASHING:
			//std::cout << "DAAAASHING\n";
			dash(dt);
			break;

		case STUNNED:
			//std::cout << "stunned\n";
			stunned(dt);
			break;

	}
}

void ChargingBoss::chase(Player& player, float dt) {
	if (target.x < getSprite().getPosition().x) {
		velocity.x = -speed;
	}
	else { 
		velocity.x = speed;
	}
	getSprite().move(velocity * dt);
	getSprite().setTextureRect(frames[0]);
}

void ChargingBoss::charge(float dt) {
	//animation de chargement de l'attaque
	if (target.x < getPosX()) dashDirection = "left";
	if (target.x > getPosX()) dashDirection = "right";
	chargingTimer += dt;
	if (chargingTimer < dashTiming - 0.25f) {
		getSprite().setTextureRect(frames[1]);
	}
	else {
		getSprite().setTextureRect(frames[2]);
	}
	if (chargingTimer > dashTiming + 0.25f) {
		chargingTimer = 0;
		currentState = DASHING;
	}
	
}

void ChargingBoss::dash(float dt) {
	if (dashDirection == "left") {
		velocity.x = -speed * 3.f;
		wallCollideSprite.setPosition(getPosX() - wallCollideSprite.getGlobalBounds().width /2, getPosY());
	}
	if (dashDirection == "right") {
		velocity.x = speed * 3.f;
		wallCollideSprite.setPosition(getPosX() + wallCollideSprite.getGlobalBounds().width / 2, getPosY());
	}
	getSprite().move(velocity * dt);
	dashTimer += dt;
	if (dashTimer > 1.f) {
		dashTimer = 0;
		currentState = CHASING;
	}
	isColliding(getWallCollideSpriteConst().getPosition().x, getWallCollideSpriteConst().getPosition().y, dt);
	if (collided) {
		currentState = STUNNED;
		dashTimer = 0;
	}
	getSprite().move(velocity* dt);

	getSprite().setTextureRect(frames[3]);
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

	getSprite().setTextureRect(frames[4]);
}

//void ChargingBoss::draw(RenderWindow& window) {
//
//}

const Sprite& ChargingBoss::getWallCollideSpriteConst() const {
	return wallCollideSprite;
}

void ChargingBoss::isColliding(int x, int y, float dt) {
	int newX = getWallCollideSpriteConst().getPosition().x + velocity.x * dt;
	int newY = getWallCollideSpriteConst().getPosition().y + velocity.y * dt;

	collided = false;

	// Vérifie la collision avant d'appliquer le mouvement
	if (map.isColliding(newX + getWallCollideSpriteConst().getGlobalBounds().width, getWallCollideSpriteConst().getGlobalBounds().top) || map.isColliding(newX + getWallCollideSpriteConst().getGlobalBounds().width, getWallCollideSpriteConst().getGlobalBounds().top + getWallCollideSpriteConst().getGlobalBounds().height) || map.isColliding(newX + getWallCollideSpriteConst().getGlobalBounds().width, getWallCollideSpriteConst().getGlobalBounds().top + getWallCollideSpriteConst().getGlobalBounds().height / 2))
	{
		newX = (static_cast<int>((getSprite().getGlobalBounds().left + getWallCollideSpriteConst().getGlobalBounds().width) / 64)) * 64 + 64 - getTexture().getSize().x / 2;
		//getSprite().setPosition(newX - 0.1, getWallCollideSpriteConst().getPosition().y);
		velocity.x = 0;
		collided = true;
	}

	else if (velocity.x < 0)
	{
		if (map.isColliding(newX, getWallCollideSpriteConst().getGlobalBounds().top) || map.isColliding(newX, getWallCollideSpriteConst().getGlobalBounds().top + getWallCollideSpriteConst().getGlobalBounds().height) || map.isColliding(newX, getWallCollideSpriteConst().getGlobalBounds().top + getWallCollideSpriteConst().getGlobalBounds().height / 2))
		{
			newX = (static_cast<int>(getSprite().getGlobalBounds().left / 64)) * 64 + getTexture().getSize().x / 2;
			//getSprite().setPosition(newX + 0.1, getWallCollideSpriteConst().getPosition().y);
			velocity.x = 0;
			collided = true;
		}
	}
	if (velocity.y > 0)
	{
		if (map.isColliding(getWallCollideSpriteConst().getGlobalBounds().left, newY + getWallCollideSpriteConst().getGlobalBounds().height) || map.isColliding(getWallCollideSpriteConst().getGlobalBounds().left + getWallCollideSpriteConst().getGlobalBounds().width, newY + getWallCollideSpriteConst().getGlobalBounds().height) || map.isColliding(getWallCollideSpriteConst().getGlobalBounds().left + getWallCollideSpriteConst().getGlobalBounds().width / 2, newY + getWallCollideSpriteConst().getGlobalBounds().height))
		{
			newY = (static_cast<int>((getSprite().getGlobalBounds().top + getWallCollideSpriteConst().getGlobalBounds().height) / 64)) * 64 + 64 - getTexture().getSize().y / 2;
			//getSprite().setPosition(getWallCollideSpriteConst().getPosition().x, newY - 0.1);
			velocity.y = 0;
			collided = true;
		}
	}
	else if (velocity.y < 0)
	{
		if (map.isColliding(getWallCollideSpriteConst().getGlobalBounds().left, newY) || map.isColliding(getWallCollideSpriteConst().getGlobalBounds().left + getWallCollideSpriteConst().getGlobalBounds().width, newY) || map.isColliding(getWallCollideSpriteConst().getGlobalBounds().left + getWallCollideSpriteConst().getGlobalBounds().width / 2, newY))
		{
			newY = (static_cast<int>(getSprite().getGlobalBounds().top / 64)) * 64 + getTexture().getSize().y / 2;
			//getSprite().setPosition(getWallCollideSpriteConst().getPosition().x, newY + 0.1);
			velocity.y = 0;
			collided = true;
		}
	}
}