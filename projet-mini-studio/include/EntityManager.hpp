#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "../include/TextureManager.hpp"
#include "../include/Player.hpp"
#include "../include/ChargingBoss.hpp"
#include "../include/EnemyFlying.hpp"
#include "../include/RangedEnemy.hpp"
#include "../include/BasicEnemy.hpp"
#include "../include/Map.hpp"

using namespace sf;
using namespace std;

class EntityManager {
public:
	EntityManager();

	TextureManager textureManager;

	shared_ptr<Player> player;
	vector<shared_ptr<Player>> playerVector;
	vector<shared_ptr<Enemy>> enemyVector;

	void createEntity(string type, Vector2f position, const Vector2f& size, const Color& color, Map& map);
	void destroyEntity();
	void collisions();

	void updateEntities(Event& event, float dt, /* Player& player1,*/ RenderWindow& window);
	void drawEntities(RenderWindow& window);

	void generateEnemies(Map& map);
};




#endif
