#ifndef CHECKPOINT_HPP
#define CHECKPOINT_HPP

#include <iostream>
#include <fstream>
#include "Entity.hpp"
#include "Player.hpp"

class Checkpoint : public Entity {
public:
	bool activated = false;
	Vector2f respawnPoint;
	Color baseColor;

	Checkpoint(const Vector2f& size, const Color& color, Map& map);

	void update();
};

class Save {
public:

	std::ifstream ifFile;
	std::string line;
	int y = 0;

	Save();

	void saveCheckpoint(const std::string& filename, std::shared_ptr<Player>& player, std::shared_ptr<Checkpoint>& checkpoint);
	void loadCheckpoint(const std::string& filename, std::shared_ptr<Player>& player);
	void reset(const std::string& filename, std::vector<std::shared_ptr<Checkpoint>> checkpointVector);
};


#endif
