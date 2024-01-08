#include "Game.h"

SpriteRenderer* Renderer;

Game::Game(unsigned int width, unsigned int height)
	: m_state(GAME_ACTIVE), m_keys(), m_width(width), m_height(height) 
{

}

Game::~Game() {

}

void Game::init() {
	ResourceManager::loadShader("shaders/sprite.ver", "shaders/sprite.frag", nullptr, "sprite");


	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInt("image", 0);
	ResourceManager::getShader("sprite").setMat("projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	ResourceManager::loadTexture("textures/ball.png", true, "ball");


}

void Game::processInput(float dt) {

}

void Game::update(float dt) {

}

void Game::render() {
	Renderer->drawSprite(ResourceManager::getTexture("face"), glm::vec2(200.0f, 200.0f), \
		glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
