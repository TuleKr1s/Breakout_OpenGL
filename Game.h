#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Sprite_Renderer.h"
#include "Resource_Manager.h"
#include "Game_Level.h"
#include "Ball_Object.h"

#include <vector>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;


class Game {
public:

	Game(unsigned int width, unsigned int heigth);
	~Game();

	std::vector<GameLevel> m_levels;
	unsigned int m_level;

	GameState m_state;
	bool m_keys[1024];
	unsigned int m_width, m_height;

	void init();

	void processInput(float dt);
	void update(float dt);
	void render();

};




#endif
