#include "Game.h"

SpriteRenderer* Renderer;

GameObject* Player;

BallObject* Ball;

ParticleGenerator* Particles;

Game::Game(unsigned int width, unsigned int height)
	: m_state(GAME_ACTIVE), m_keys(), m_width(width), m_height(height) 
{

}

Game::~Game() {
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;

}

void Game::init() {

	ResourceManager::loadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShader("shaders/particle.vert", "shaders/particle.frag", nullptr, "particle");


	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInt("image", 0);
	ResourceManager::getShader("sprite").setMat("projection", projection);
	ResourceManager::getShader("particle").use().setInt("sprite", 0);
	ResourceManager::getShader("particle").use().setMat("projection", projection);

	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	// loading textures
	ResourceManager::loadTexture("textures/ball.png", true, "ball");
	ResourceManager::loadTexture("textures/background.jpg", false, "background");
	ResourceManager::loadTexture("textures/block.png", false, "block");
	ResourceManager::loadTexture("textures/block_solid.png", false, "block_solid");
	ResourceManager::loadTexture("textures/paddle.png", true, "paddle");
	ResourceManager::loadTexture("textures/particle.png", true, "particle");

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

	Particles = new ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500);
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

void Game::resetLevel() {
	if (m_level == 0)
		m_levels[0].load("levels/one.lvl", m_width, m_height / 2);
	else if (m_level == 1)
		m_levels[1].load("levels/two.lvl", m_width, m_height / 2);
	else if (m_level == 2)
		m_levels[2].load("levels/three.lvl", m_width, m_height / 2);
	else if (m_level == 3)
		m_levels[3].load("levels/four.lvl", m_width, m_height / 2);
}

void Game::resetPlayer() {
	Player->m_size = PLAYER_SIZE;
	Player->m_position = glm::vec2(m_width / 2.0f - PLAYER_SIZE.x / 2.0f, m_height - PLAYER_SIZE.y);
	Ball->reset(Player->m_position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

void Game::update(float dt) {
	Ball->move(dt, m_width);

	doCollision();

	Particles->update(dt, *Ball, 2, glm::vec2(Ball->m_radius / 2.0f));

	if (Ball->m_position.y >= m_height) {
		resetLevel();
		resetPlayer();
	}
	
}

void Game::render() {

	if (m_state == GAME_ACTIVE) {
		Renderer->drawSprite(ResourceManager::getTexture("background"), \
			glm::vec2(0.0f, 0.0f), glm::vec2(m_width, m_height), 0.0f);

		m_levels[m_level].draw(*Renderer);

		Particles->draw();

		Player->draw(*Renderer);

		Ball->draw(*Renderer);
		
	}

}
bool checkCollision(GameObject& one, GameObject& two);
Collision checkCollision(BallObject& one, GameObject& two);
Direction vectorDirection(glm::vec2 target);

void Game::doCollision() {
	for (GameObject& box : m_levels[m_level].m_bricks) {
		if (!box.m_destroyed) {
			Collision collision = checkCollision(*Ball, box);
			if (std::get<0>(collision)) {
				if (!box.m_isSolid)
					box.m_destroyed = true;
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);

				if (dir == LEFT || dir == RIGHT) {
					Ball->m_velocity.x = -Ball->m_velocity.x;

					float penetration = Ball->m_radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->m_position.x += penetration;
					else
						Ball->m_position.x -= penetration;
				}
				else {
					Ball->m_velocity.y = -Ball->m_velocity.y;
					float penetration = Ball->m_radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->m_position.y -= penetration;
					else
						Ball->m_position.y += penetration;
				}
			}
		}
	}
	Collision result = checkCollision(*Ball, *Player);
	if (!Ball->m_stuck && std::get<0>(result)) {
		float centerBoard = Player->m_position.x + Player->m_size.x / 2.0f;
		float distance = (Ball->m_position.x + Ball->m_radius) - centerBoard;
		float percentage = distance / (Player->m_size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->m_velocity;
		Ball->m_velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->m_velocity.y = -1.0f * abs(Ball->m_velocity.y);
		Ball->m_velocity = glm::normalize(Ball->m_velocity) * glm::length(oldVelocity);
	}
}

bool checkCollision(GameObject& one, GameObject& two) {

	bool collisionX = one.m_position.x + one.m_size.x >= two.m_position.x && two.m_position.x + two.m_size.x >= one.m_position.x;
	bool collisionY = one.m_position.y + one.m_size.y >= two.m_position.y && two.m_position.y + two.m_size.y >= one.m_position.y;

	return collisionX && collisionY;
}

Collision checkCollision(BallObject& one, GameObject& two) {
	glm::vec2 center(one.m_position + one.m_radius);

	glm::vec2 aabb_half_extents(two.m_size.x / 2.0f, two.m_size.y / 2.0f);
	glm::vec2 aabb_center(two.m_position.x + aabb_half_extents.x, two.m_position.y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;
	if (glm::length(difference) <= one.m_radius)
		return std::make_tuple(true, vectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

Direction vectorDirection(glm::vec2 target) {
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f), // down
		glm::vec2(-1.0f, 0.0f)	// left
	};

	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; ++i) {
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max) {
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}