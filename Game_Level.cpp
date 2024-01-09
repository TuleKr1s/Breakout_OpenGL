#include "Game_Level.h"

void GameLevel::load(const char* file, unsigned int levelWidth, unsigned int levelHeight) {

	// clearing old data
	m_bricks.clear();

	unsigned int tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;
	if (fstream) {
		while (std::getline(fstream, line)) {
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
	else {
		std::cerr << "Failed to load level data file: " << file << std::endl;
	}

}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight) {

	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / static_cast<float>(height);

	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {

			// check for block type
			if (tileData[y][x] == 1) { //solid 
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::getTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
				obj.m_isSolid = true;
				m_bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1) {
				glm::vec3 color = glm::vec3(1.0f); // original color white
				switch (tileData[y][x]) {
				case 2:
					color = glm::vec3(0.2f, 0.6f, 1.0f); // sky blue
					break;
				case 3:
					color = glm::vec3(0.0f, 0.7f, 0.0f); // light green
					break;
				case 4:
					color = glm::vec3(0.8f, 0.8f, 0.4f); // yellow
					break;
				case 5:
					color = glm::vec3(1.0f, 0.5f, 0.0f); // orange
					break;
				}

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				m_bricks.push_back(GameObject(pos, size, ResourceManager::getTexture("block"), color));

			}
		}
	}
}

void GameLevel::draw(SpriteRenderer& renderer) {
	for (GameObject& tile : m_bricks)
		if (!tile.m_destroyed)
			tile.draw(renderer);
}

bool GameLevel::isCompleted() {
	for (GameObject& tile : m_bricks)
		if (!tile.m_isSolid && !tile.m_destroyed)
			return false;
	return true;
}