#include "../include/Checkpoint.hpp"


Checkpoint::Checkpoint(const Vector2f& size, const Color& color, Map& map) : Entity(size, color) {
	baseColor = color;
}

void Checkpoint::update() {
	if (activated) {
		getSprite().setColor(Color::Red);
	}
	else {
		getSprite().setColor(baseColor);
	}
}

Save::Save() {

}

void Save::saveCheckpoint(const std::string& filename, std::shared_ptr<Player>& player, std::shared_ptr<Checkpoint>& checkpoint){
	std::ofstream file(filename);
	if (file.is_open()) {
		file << checkpoint->respawnPoint.x << std::endl << checkpoint->respawnPoint.y << std::endl;
	}
	else {
		std::cerr << "Impossible de sauvegarder le checkpoint." << std::endl;
	}
}

void Save::loadCheckpoint(const std::string& filename, std::shared_ptr<Player>& player) {
	ifFile.open(filename);
	y = 0;
	while (getline(ifFile, line)) {
		if (y == 0) {
			player->setPosX(std::stof(line));
		}
		else if (y == 1) {
			player->setPosY(std::stof(line));
		}
		y++;
	}
}

void Save::reset(const std::string& filename, std::vector<std::shared_ptr<Checkpoint>> checkpointVector) {
	std::ofstream file(filename);
	if (file.is_open()) {
		file << 0 << std::endl << 0 << std::endl;
	}
	else {
		std::cerr << "Impossible de sauvegarder le checkpoint." << std::endl;
	}
	for (auto& checkpoint : checkpointVector) {
		checkpoint->activated = false;
	}
}