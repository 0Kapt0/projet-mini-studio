#ifndef CHECKPOINT_HPP
#define CHECKPOINT_HPP

#include <iostream>
#include <fstream>
#include "Entity.hpp"
#include "Player.hpp"

class Checkpoint : public Entity {
public:
	bool activated = false;
	bool animating = false;
	Vector2f respawnPoint;
	Color baseColor;

	Checkpoint(Texture& texture);
	Checkpoint(const Vector2f& size, const Color& color, Map& map);

	//void update();
	void activate();
	void setTexture(Texture& tex, int frameWidth, int frameHeight, int totalFrames, float frameDuration) override;
	void animate(float deltaTime) override;
};

class Save {
public:

	std::ifstream ifFile;
	std::string line;
	std::vector<std::string> getLine;

	Save();

	void saveCheckpoint(const std::string& filename, std::shared_ptr<Player>& player, std::shared_ptr<Checkpoint>& checkpoint);
	void loadCheckpoint(const std::string& filename, std::shared_ptr<Player>& player);
	void reset(const std::string& filename, std::vector<std::shared_ptr<Checkpoint>> checkpointVector);
	void playerDied(const std::string& filename, std::shared_ptr<Player>& player);
	std::vector<std::string> getSaveLines(const std::string& filenames);
	void resetPlayerPos(const std::string& filename);
	void win(const std::string& filename, std::shared_ptr<Player>& player, int levelUnlocked);

	bool cinematic1Played = false;
	bool cinematic2Played = false;
	bool cinematic3Played = false;
	bool cinematic4Played = false;
	bool level2Unlocked = false;
	bool level3Unlocked = false;
};


#endif
