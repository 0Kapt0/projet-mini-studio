#include "../include/Checkpoint.hpp"

Checkpoint::Checkpoint(Texture& texture) {
	getSprite().setTexture(texture);
}

Checkpoint::Checkpoint(const Vector2f& size, const Color& color, Map& map) : Entity(size, color) {
	baseColor = color;
}

//void Checkpoint::update() {
//	if (activated) {
//		getSprite().setColor(Color::Red);
//	}
//	else {
//		getSprite().setColor(baseColor);
//	}
//}

void Checkpoint::activate() {
	activated = true;
	animating = true;
}

void Checkpoint::setTexture(Texture& tex, int frameWidth, int frameHeight, int _totalFrames, float _frameTime) {
	getSprite().setTexture(tex);
	getSprite().setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
	getSprite().setOrigin(frameWidth / 2, frameHeight / 2);

	frames.clear();
	for (int i = 0; i < _totalFrames; ++i) {
		frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
	}

	totalFrames = _totalFrames;
	frameTime = _frameTime;
	getSprite().setTextureRect(frames[currentFrame]);
	getSprite().setOrigin(getSprite().getTextureRect().getSize().x / 2, getSprite().getTextureRect().getSize().y / 2);
}
void Checkpoint::animate(float deltaTime) {
	if (activated && animating) {
		elapsedTime += deltaTime;
		if (elapsedTime >= frameTime && !frames.empty()) {
			elapsedTime = 0.0f;

			if (currentFrame < totalFrames - 1) {
				currentFrame++;
			}
		}
	}
	if (!activated) {
		currentFrame = 0;
	}
	getSprite().setTextureRect(frames[currentFrame]);
}

Save::Save() {

}

void Save::saveCheckpoint(const std::string& filename, std::shared_ptr<Player>& player, std::shared_ptr<Checkpoint>& checkpoint) {
	getLine.clear();
	getLine.push_back(std::to_string(checkpoint->respawnPoint.x));
	getLine.push_back(std::to_string(checkpoint->respawnPoint.y));
	getLine.push_back(std::to_string(player->getMaxHp()));
	getLine.push_back(std::to_string(player->doubleJumpUnlocked));
	getLine.push_back(std::to_string(player->dashUnlocked));
	getLine.push_back(std::to_string(player->grappleUnlocked));
	getLine.push_back(std::to_string(cinematic1Played));
	getLine.push_back(std::to_string(cinematic2Played));
	getLine.push_back(std::to_string(cinematic3Played));
	getLine.push_back(std::to_string(cinematic4Played));
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
	while (getline(ifFile, line)) {
		getLine.push_back(line);
	}
	ifFile.close();
	player->setPosX(std::stof(getLine[0]));
	player->setPosY(std::stof(getLine[1]));
	player->setMaxHp(std::stoi(getLine[2]));
	player->hp = player->getMaxHp();
	player->doubleJumpUnlocked = (std::stoi(getLine[3]));
	player->dashUnlocked = (std::stoi(getLine[4]));
	player->grappleUnlocked = (std::stoi(getLine[5]));
	cinematic1Played = (std::stoi(getLine[6]));
	cinematic2Played = (std::stoi(getLine[7]));
	cinematic3Played = (std::stoi(getLine[8]));
	cinematic4Played = (std::stoi(getLine[9]));
}

void Save::reset(const std::string& filename, std::vector<std::shared_ptr<Checkpoint>> checkpointVector) {
	getLine.clear();
	getLine.push_back(std::to_string(128)); //pos x
	getLine.push_back(std::to_string(1800)); // pos  y
	getLine.push_back(std::to_string(3)); // max hp
	getLine.push_back(std::to_string(0)); // doublejump unlocked
	getLine.push_back(std::to_string(0)); // dash unlocked
	getLine.push_back(std::to_string(0)); // grapple unlocked
	getLine.push_back(std::to_string(0)); // cinematic1 played unlocked
	getLine.push_back(std::to_string(0)); // cinematic2 played unlocked
	getLine.push_back(std::to_string(0)); // cinematic3 played unlocked
	getLine.push_back(std::to_string(0)); // cinematic4 played unlocked
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