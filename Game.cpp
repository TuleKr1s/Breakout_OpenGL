#include "Game.h"

SpriteRenderer* Renderer;

GameObject* Player;

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

	glm::vec2 playerPos = glm::vec2(m_width / 2.0f - PLAYER_SIZE.x / 2.0f, \
		m_height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));


}

void Game::processInput(float dt) {
	
	float velocity = PLAYER_VELOCITY * dt;

	// Перемещаем ракетку
	if (this->m_keys[GLFW_KEY_A])
	{
		if (Player->m_position.x >= 0.0f)
			Player->m_position.x -= velocity;
	}
	if (this->m_keys[GLFW_KEY_D])
	{
		if (Player->m_position.x <= this->m_width - Player->m_size.x)
			Player->m_position.x += velocity;
	}
}

void Game::update(float dt) {

}

void Game::render() {

	if (m_state == GAME_ACTIVE) {
		Renderer->drawSprite(ResourceManager::getTexture("background"), \
			glm::vec2(0.0f, 0.0f), glm::vec2(m_width, m_height), 0.0f);

		m_levels[m_level].draw(*Renderer);

		Player->draw(*Renderer);
	}

}
