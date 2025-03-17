#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "../include/TextureManager.hpp"
#include "../include/Player.hpp"
#include "../include/ChargingBoss.hpp"
#include "../include/EnemyFlying.hpp"
#include "../include/RangedEnemy.hpp"
#include "../include/BasicEnemy.hpp"
#include "Checkpoint.hpp"

using namespace sf;

class EntityManager {
public:
	EntityManager();

	TextureManager textureManager;

	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Player>> playerVector;
	std::vector<std::shared_ptr<Enemy>> enemyVector;
	std::vector<std::shared_ptr<Checkpoint>> checkpointVector;

	float timer = 0;

	Save save;

	void createEntity(std::string type, Vector2f position, const Vector2f& size, const Color& color, Map& map);
	void destroyEntity();
	void collisions();

	void updateEntities(Event& event, float dt, /* Player& player1,*/ RenderWindow& window);
	void drawEntities(RenderWindow& window);
};




#endif
