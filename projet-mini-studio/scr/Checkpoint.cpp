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
	getLine.clear();
	getLine.push_back(std::to_string(checkpoint->respawnPoint.x));
	getLine.push_back(std::to_string(checkpoint->respawnPoint.y));
	getLine.push_back(std::to_string(player->getMaxHp()));
	getLine.push_back(std::to_string(player->doubleJumpUnlocked));
	getLine.push_back(std::to_string(player->dashUnlocked));
	getLine.push_back(std::to_string(player->grappleUnlocked));
	std::ofstream file(filename);
	if (file.is_open()) {
		for (auto& newLine : getLine) {
			file << newLine << std::endl;
		}
	}
	else {
		std::cerr << "Impossible de sauvegarder le checkpoint." << std::endl;
	}
}

void Save::loadCheckpoint(const std::string& filename, std::shared_ptr<Player>& player) {
	getLine.clear();
	ifFile.open(filename);
	y = 0;
	while (getline(ifFile, line)) {
		getLine.push_back(line);
		/*if (y == 0) {
			player->setPosX(std::stof(line));
		}
		else if (y == 1) {
			player->setPosY(std::stof(line));
		}
		else if (y == 2) {
			player->setMaxHp(std::stoi(line));
			player->hp = player->getMaxHp();
		}
		else if (y == 3) {
			player->doubleJumpUnlocked = (std::stoi(line));
		}
		else if (y == 4) {
			player->dashUnlocked = (std::stoi(line));
		}
		else if (y == 5) {
			player->grappleUnlocked = (std::stoi(line));
		}
		y++;*/
	}
	ifFile.close();
	player->setPosX(std::stof(getLine[0]));
	player->setPosY(std::stof(getLine[1]));
	player->setMaxHp(std::stoi(getLine[2]));
	player->hp = player->getMaxHp();
	player->doubleJumpUnlocked = (std::stoi(getLine[3]));
	player->dashUnlocked = (std::stoi(getLine[4]));
	player->grappleUnlocked = (std::stoi(getLine[5]));
}

void Save::reset(const std::string& filename, std::vector<std::shared_ptr<Checkpoint>> checkpointVector) {
	getLine.clear();
	getLine.push_back(std::to_string(0));
	getLine.push_back(std::to_string(0));
	getLine.push_back(std::to_string(3));
	getLine.push_back(std::to_string(0));
	getLine.push_back(std::to_string(0));
	getLine.push_back(std::to_string(0));
	std::ofstream file(filename);
	if (file.is_open()) {
		for (auto& newLine : getLine) {
			file << newLine << std::endl;
		}
	}
	else {
		std::cerr << "Impossible de sauvegarder le checkpoint." << std::endl;
	}
	for (auto& checkpoint : checkpointVector) {
		checkpoint->activated = false;
	}
}

void Save::playerDied(const std::string& filename, std::shared_ptr<Player>& player) {
	player->setMaxHp(player->getMaxHp() - 1);
	player->hp = player->getMaxHp();
	player->killCount = 0;

	getLine.clear();
	ifFile.open(filename);
	while (getline(ifFile, line)) {
		getLine.push_back(line);
	}
	ifFile.close();

	getLine[2] = std::to_string(player->getMaxHp());

	std::ofstream file(filename);
	if (file.is_open()) {
		for (auto& newLine : getLine) {
			file << newLine << std::endl;
		}
	}
	else {
		std::cerr << "Impossible de sauvegarder le checkpoint." << std::endl;
	}
}