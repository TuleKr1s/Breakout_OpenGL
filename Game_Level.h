#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Game_Object.h"
#include "Sprite_Renderer.h"
#include "Resource_Manager.h"

#include <vector>

class GameLevel {
public:
	std::vector<GameObject> m_bricks;

	GameLevel() {}

	void load(const char* file, unsigned int levelWidth, unsigned int levelHeight);

	void draw(SpriteRenderer& renderer);

	bool isCompleted();

private:

	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};



#endif