#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "../include/TextureManager.hpp"
#include "../include/Player.hpp"
#include "../include/ChargingBoss.hpp"
#include "../include/EnemyFlying.hpp"
#include "../include/RangedEnemy.hpp"
#include "../include/BasicEnemy.hpp"
#include "../include/Map.hpp"
#include "Checkpoint.hpp"
#include "HeartItem.hpp"
#include "../include/FlyingBoss.hpp"
#include "FinalBoss.hpp"

using namespace sf;
using namespace std;

class EntityManager {
public:
	EntityManager();

	TextureManager textureManager;
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Player>> playerVector;
	std::vector<std::shared_ptr<Enemy>> enemyVector;
	std::vector<std::shared_ptr<Checkpoint>> checkpointVector;
	std::vector<std::shared_ptr<Item>> itemVector;

	float timer = 0;

	Save save;

	bool win = false;

	void createEntity(string type, Vector2f position, const Vector2f& size, const Color& color, Map& map);
	void destroyEntity();
	void collisions(float dt);

	void updateEntities(Event& event, float dt, /* Player& player1,*/ RenderWindow& window);
	void drawEntities(RenderWindow& window);

	void generateEnemies(Map& map);
};




#endif
