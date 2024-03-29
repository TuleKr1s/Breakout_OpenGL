#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <irrKlang/irrKlang.h>
using namespace irrklang;

#include "Sprite_Renderer.h"
#include "Resource_Manager.h"
#include "Game_Level.h"
#include "Ball_Object.h"
#include "Particle_Generator.h"
#include "Post_Processor.h"
#include "Power_Up.h"
#include "Text_Renderer.h"

#include <vector>
#include <tuple>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game {
public:

	Game(unsigned int width, unsigned int heigth);
	~Game();

	unsigned int m_lives;

	std::vector<GameLevel> m_levels;
	unsigned int m_level;

	GameState m_state;
	bool m_keys[1024];
	bool m_keysProcessed[1024];
	unsigned int m_width, m_height;

	std::vector<PowerUp> PowerUps;

	void init();
	void resetLevel();
	void resetPlayer();

	void processInput(float dt);
	void update(float dt);
	void render();

	void doCollision();

	void spawnPowerUps(GameObject& block);
	void updatePowerUps(float dt);

};




#endif
