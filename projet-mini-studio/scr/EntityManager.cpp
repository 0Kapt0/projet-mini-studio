#include "../include/EntityManager.hpp"

EntityManager::EntityManager() {

}

void EntityManager::createEntity(std::string type, Vector2f position, const Vector2f& size, const Color& color, Map& map) {
	if (type == "Player") {
		std::shared_ptr<Player> _player = std::make_shared<Player>(Vector2f(50, 50), Color::Red, map);
		_player->getSprite().setPosition(position);
		player = _player;
	}
	if (type == "EnemyFlying") {
		std::shared_ptr<EnemyFlying> eFlying = std::make_shared<EnemyFlying>(Vector2f(50, 50), Color::Green, map);
		eFlying->getSprite().setPosition(position);
		enemyVector.push_back(eFlying);
	}
	if (type == "RangedEnemy") {
		std::shared_ptr<RangedEnemy> eRanged = std::make_shared<RangedEnemy>(Vector2f(50, 50), Color::Yellow, map);
		eRanged->getSprite().setPosition(position);
		enemyVector.push_back(eRanged);
	}
	if (type == "BasicEnemy") {
		std::shared_ptr<BasicEnemy> eBasic = std::make_shared<BasicEnemy>(Vector2f(50, 50), Color::Blue, map);
		eBasic->getSprite().setPosition(position);
		enemyVector.push_back(eBasic);
	}
	if (type == "ChargingBoss") {
		std::shared_ptr<ChargingBoss> chargingBoss = std::make_shared<ChargingBoss>(Vector2f(100, 100), Color(239, 12, 197), map);
		chargingBoss->getSprite().setPosition(position);
		enemyVector.push_back(chargingBoss);
	}
}

void EntityManager::destroyEntity() {
	enemyVector.erase(remove_if(enemyVector.begin(), enemyVector.end(),
			[](const shared_ptr<Enemy>& enemy) { return enemy->toBeDeleted; }),
		enemyVector.end());
}

void EntityManager::collisions() {
	for (auto& enemy : enemyVector) {
		if (player->getAttackHitBox().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) && player->attacking && !enemy->invincible) {
			//DEGATS
			enemy->toBeDeleted = true;
		}
		if (player->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) && !player->invincible) {
			player->invincible = true;
			std::cout << "DAMAGE" << std::endl;
		}
	}
}

void EntityManager::updateEntities(Event& event, float dt, /* Player& player1,*/ RenderWindow& window) {
	collisions();
	player->update(dt);
	player->handleInput(event, window, dt);
	for (auto& enemy : enemyVector) {
		enemy->update(dt, *player, window);
	}
}

void EntityManager::drawEntities(RenderWindow& window) {
	player->draw(window);
	for (auto& enemy : enemyVector) {
		enemy->draw(window);
	}
}