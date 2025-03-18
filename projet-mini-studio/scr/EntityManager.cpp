#include "../include/EntityManager.hpp"

EntityManager::EntityManager() {

}

void EntityManager::createEntity(string type, Vector2f position, const Vector2f& size, const Color& color, Map& map) {
	if (type == "Player") {
		shared_ptr<Player> _player = make_shared<Player>(size, color, map);
		_player->getSprite().setPosition(position);
		player = _player;
	}
	if (type == "EnemyFlying") {
		shared_ptr<EnemyFlying> eFlying = make_shared<EnemyFlying>(size, color, map);
		eFlying->getSprite().setPosition(position);
		enemyVector.push_back(eFlying);
	}
	if (type == "RangedEnemy") {
		shared_ptr<RangedEnemy> eRanged = make_shared<RangedEnemy>(size, color, map);
		eRanged->getSprite().setPosition(position);
		enemyVector.push_back(eRanged);
	}
	if (type == "BasicEnemy") {
		shared_ptr<BasicEnemy> eBasic = make_shared<BasicEnemy>(size, color, map);
		eBasic->getSprite().setPosition(position);
		enemyVector.push_back(eBasic);
	}
	if (type == "ChargingBoss") {
		shared_ptr<ChargingBoss> chargingBoss = make_shared<ChargingBoss>(size, color, map);
		chargingBoss->getSprite().setPosition(position);
		enemyVector.push_back(chargingBoss);
	}
}

void EntityManager::generateEnemies(Map& map) {
	enemyVector.clear();

	for (const auto& spawn : map.enemySpawns) {
		Vector2f position(spawn.x, spawn.y);
		Vector2f size(50, 50);

		Color color;
		if (spawn.type == "EnemyFlying") {
			color = Color::Green;
		}
		else if (spawn.type == "RangedEnemy") {
			color = Color::Yellow;
		}
		else if (spawn.type == "BasicEnemy") {
			color = Color::Blue;
		}
		else if (spawn.type == "ChargingBoss") {
			color = Color(239, 12, 197);
		}
		else {
			color = Color::White;
		}
		createEntity(spawn.type, position, size, color, map);
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
			cout << "DAMAGE" << endl;
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