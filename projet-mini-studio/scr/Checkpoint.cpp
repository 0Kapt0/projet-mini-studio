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
	//sprite.setSize(Vector2f(frameWidth, frameHeight));
	getSprite().setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
	//getSprite().setOrigin(frameWidth / 2, 0);
	getSprite().setOrigin(frameWidth / 2, frameHeight / 2);

	frames.clear();
	for (int i = 0; i < _totalFrames; ++i) {
		frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
	}

	totalFrames = _totalFrames;
	frameTime = _frameTime;
	getSprite().setTextureRect(frames[currentFrame]);
}
void Checkpoint::animate(float deltaTime) {
	if (activated && animating) {
		elapsedTime += deltaTime;
		if (elapsedTime >= frameTime && !frames.empty()) {
			elapsedTime = 0.0f;

			if (currentFrame < totalFrames - 1) {
				currentFrame++;
			}
			else {
				cout << "Animation terminée, blocage sur la dernière frame." << std::endl;
				return;
			}

			//getSprite().setTextureRect(frames[currentFrame]);
		}
	}
	if (activated && !animating) {
		currentFrame = totalFrames - 1; //la dernière
	}
	if (!activated) {
		currentFrame = 0;
	}
	getSprite().setTextureRect(frames[currentFrame]);
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