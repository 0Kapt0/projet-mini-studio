#include "../include/EntityManager.hpp"

EntityManager::EntityManager() {

}

void EntityManager::createEntity(string type, Vector2f position, const Vector2f& size, const Color& color, Map& map) {
	if (type == "Player") {
		std::shared_ptr<Player> _player = std::make_shared<Player>(textureManager.playerTexture, map);
		//_player->getSprite().setPosition(position);
		_player->setTexture(textureManager.playerTexture, 0, 0, 0, 0.1f);
		player = _player;
		save.loadCheckpoint("assets/checkpoint/player.txt", player);
	}
	if (type == "EnemyFlying") {
		std::shared_ptr<EnemyFlying> eFlying = std::make_shared<EnemyFlying>(size, color, map);
		eFlying->getSprite().setPosition(position);
		enemyVector.push_back(eFlying);
	}
	if (type == "RangedEnemy") {
		std::shared_ptr<RangedEnemy> eRanged = std::make_shared<RangedEnemy>(size, color, map);
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
		testCheckpoint->setTexture(textureManager.checkpointTexture, 134, 136, 4, 0.1f);
		checkpointVector.push_back(testCheckpoint);
	}
	if (type == "HeartItem") {
		std::shared_ptr<HeartItem> heartItem = std::make_shared<HeartItem>(textureManager.heartTexture);
		heartItem->getSprite().setPosition(position);
		itemVector.push_back(heartItem);
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
	itemVector.erase(remove_if(itemVector.begin(), itemVector.end(),
		[](const shared_ptr<Item>& item) { return item->toBeDeleted; }),
		itemVector.end());
}

void EntityManager::collisions() {
	for (auto& enemy : enemyVector) {
		if (player->getAttackHitBox().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) && player->isAttacking() && !enemy->invincible) {
			//DEGATS
			enemy->hp--;
			if (enemy->hp <= 0) {
				enemy->toBeDeleted = true;
				player->killCount++;
			}
			else {
				enemy->invincible = true;
			}
		}
		if (player->getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds()) && !player->invincible) {
			player->invincible = true;
			player->hp--;
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
	if (!itemVector.empty()) {
		for (auto& item : itemVector) {
			if (player->getSprite().getGlobalBounds().intersects(item->getSprite().getGlobalBounds())) {
				if (item->type == "HeartItem" && player->hp < player->getMaxHp()) {
					player->hp++;
					item->toBeDeleted = true;
				}
			}
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
	if (player->hp <= 0) {
		player->hp = 0;
	}
	collisions();
	player->update(dt);
	player->handleInput(event, window, dt);
	player->animate(dt);
	for (auto& enemy : enemyVector) {
		enemy->update(dt, *player, window);
		/*if (enemy->hp <= 0) {
			enemy->toBeDeleted;
			player->killCount++;
		}*/
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
	for (auto& item : itemVector) {
		item->draw(window);
	}
	player->draw(window);
}