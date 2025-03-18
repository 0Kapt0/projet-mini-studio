#include "../include/EntityManager.hpp"

EntityManager::EntityManager() {

}

void EntityManager::createEntity(string type, Vector2f position, const Vector2f& size, const Color& color, Map& map) {
	if (type == "Player") {
		shared_ptr<Player> _player = make_shared<Player>(size, color, map);
		_player->getSprite().setPosition(position);
		player = _player;
		save.loadCheckpoint("assets/checkpoint/player.txt", player);
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
	if (type == "Checkpoint") {
		std::shared_ptr<Checkpoint> testCheckpoint = std::make_shared<Checkpoint>(size, color, map);
		testCheckpoint->getSprite().setPosition(position);
		testCheckpoint->setTexture(textureManager.checkpointTexture, textureManager.checkpointTexture.getSize().x / 4, 
			textureManager.checkpointTexture.getSize().y, 4, 0.1f);
		checkpointVector.push_back(testCheckpoint);
		std::cout << textureManager.checkpointTexture.getSize().x / 4 << std::endl;
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
		if (player->getAttackHitBox().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) && player->isAttacking() && !enemy->invincible) {
			//DEGATS
			enemy->toBeDeleted = true;
			player->killCount++;
		}
		if (player->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) && !player->invincible) {
				player->invincible = true;
			//std::cout << "DAMAGE" << std::endl;
		}
	}
	for (auto& checkpoint : checkpointVector) {
		if (player->getSprite().getGlobalBounds().intersects(checkpoint->getSprite().getGlobalBounds())) {
			checkpoint->respawnPoint = Vector2f(checkpoint->getPosX(), checkpoint->getPosY() - player->getHeight()/2 + checkpoint->getHeight()/2);
			save.saveCheckpoint("assets/checkpoint/player.txt", player, checkpoint);
			/*std::for_each(checkpointVector.begin(), checkpointVector.end(), [](std::shared_ptr<Checkpoint>& obj) {
				if (obj->activated) {
					obj->activated = false;
				}
			});*/
			for (auto& other : checkpointVector) {
				if (other->activated) {
					other->activated = false;
				}
			}
			checkpoint->activate();
		}
	}
}

void EntityManager::updateEntities(Event& event, float dt, /* Player& player1,*/ RenderWindow& window) {
	if (player->hp > player->getMaxHp()) {
		player->hp = player->getMaxHp();
	}
	if (player->getMaxHp() < player->hpCeiling) {
		if (player->killCount == 3) {
			player->oneUp(1);
			player->killCount = 0;
		}
	}
	else {
		player->setMaxHp(player->hpCeiling);
	}
	collisions();
	player->update(dt);
	player->handleInput(event, window, dt);
	for (auto& enemy : enemyVector) {
		enemy->update(dt, *player, window);
	}
	for (auto& checkpoint : checkpointVector) {
		checkpoint->animate(dt);
	}
	/*if (Keyboard::isKeyPressed(Keyboard::R)) {
		save.reset("assets/checkpoint/player.txt", checkpointVector);
	}
	timer += dt;
	if (Keyboard::isKeyPressed(Keyboard::K) && timer > 1.f) {
		save.playerDied("assets/checkpoint/player.txt", player);
		timer = 0;
	}
	if (Keyboard::isKeyPressed(Keyboard::L) && timer > 1.f) {
		player->hp += 1;
		timer = 0;
	}
	if (Keyboard::isKeyPressed(Keyboard::M) && timer > 1.f) {
		player->oneUp(1);
		timer = 0;
	}*/
}

void EntityManager::drawEntities(RenderWindow& window) {
	for (auto& enemy : enemyVector) {
		enemy->draw(window);
	}
	for (auto& checkpoint : checkpointVector) {
		checkpoint->draw(window);
	}
	player->draw(window);
}