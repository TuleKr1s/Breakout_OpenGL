#include "Game.h"

SpriteRenderer* Renderer;

GameObject* Player;

BallObject* Ball;

Game::Game(unsigned int width, unsigned int height)
	: m_state(GAME_ACTIVE), m_keys(), m_width(width), m_height(height) 
{

}

Game::~Game() {

}

void Game::init() {
	ResourceManager::loadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");


	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInt("image", 0);
	ResourceManager::getShader("sprite").setMat("projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	// loading textures
	ResourceManager::loadTexture("textures/ball.png", true, "ball");
	ResourceManager::loadTexture("textures/background.jpg", false, "background");
	ResourceManager::loadTexture("textures/block.png", false, "block");
	ResourceManager::loadTexture("textures/block_solid.png", false, "block_solid");
	ResourceManager::loadTexture("textures/paddle.png", true, "paddle");

	// loading levels
	GameLevel one;
	GameLevel two;
	GameLevel three;
	GameLevel four;
	
	one.load("levels/one.lvl", m_width, m_height / 2);
	two.load("levels/two.lvl", m_width, m_height / 2);
	three.load("levels/three.lvl", m_width, m_height / 2);
	four.load("levels/four.lvl", m_width, m_height / 2);

	m_levels.push_back(one);
	m_levels.push_back(two);
	m_levels.push_back(three);
	m_levels.push_back(four);
	m_level = 0;

	// player settings
	glm::vec2 playerPos = glm::vec2(m_width / 2.0f - PLAYER_SIZE.x / 2.0f, \
		m_height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

	// ball settings
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("ball"));
}

void Game::processInput(float dt) {
	
	float velocity = PLAYER_VELOCITY * dt;

	// Перемещаем ракетку
	if (this->m_keys[GLFW_KEY_A])
	{
		if (Player->m_position.x >= 0.0f) {
			Player->m_position.x -= velocity;
			if (Ball->m_stuck)
				Ball->m_position.x -= velocity;
		}
	}
	if (this->m_keys[GLFW_KEY_D])
	{
		if (Player->m_position.x <= this->m_width - Player->m_size.x) {
			Player->m_position.x += velocity;
			if (Ball->m_stuck)
				Ball->m_position.x += velocity;
		}
	}
	if (this->m_keys[GLFW_KEY_SPACE])
		Ball->m_stuck = false;
}

void Game::update(float dt) {
	Ball->move(dt, m_width);

	doCollision();
}

void Game::render() {

	if (m_state == GAME_ACTIVE) {
		Renderer->drawSprite(ResourceManager::getTexture("background"), \
			glm::vec2(0.0f, 0.0f), glm::vec2(m_width, m_height), 0.0f);

		m_levels[m_level].draw(*Renderer);

		Player->draw(*Renderer);

		Ball->draw(*Renderer);
	}

}
bool checkCollision(GameObject& one, GameObject& two);
bool checkCollision(BallObject& one, GameObject& two);

void Game::doCollision() {
	for (GameObject& box : m_levels[m_level].m_bricks) {
		if (!box.m_destroyed) {
			if (checkCollision(*Ball, box)) {
				if (!box.m_isSolid)
					box.m_destroyed = true;
			}
		}
	}
}

bool checkCollision(GameObject& one, GameObject& two) {

	bool collisionX = one.m_position.x + one.m_size.x >= two.m_position.x && two.m_position.x + two.m_size.x >= one.m_position.x;
	bool collisionY = one.m_position.y + one.m_size.y >= two.m_position.y && two.m_position.y + two.m_size.y >= one.m_position.y;

	return collisionX && collisionY;
}

bool checkCollision(BallObject& one, GameObject& two) {
	glm::vec2 center(one.m_position + one.m_radius);

	glm::vec2 aabb_half_extents(two.m_size.x / 2.0f, two.m_size.y / 2.0f);
	glm::vec2 aabb_center(two.m_position.x + aabb_half_extents.x, two.m_position.y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;
	return glm::length(difference) < one.m_radius;
}