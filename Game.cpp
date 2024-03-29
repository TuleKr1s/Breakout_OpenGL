#include "Game.h"

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;
TextRenderer* Text;

ISoundEngine* SoundEngine = createIrrKlangDevice();

float shakeTime = 0.0f;
float soundVolume = 0.4f;

Game::Game(unsigned int width, unsigned int height)
	: m_state(GAME_MENU), m_keys(), m_width(width), m_height(height), m_lives(3)
{

}

Game::~Game() {
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	delete Text;
}

void Game::init() {

	// loading shaders
	ResourceManager::loadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShader("shaders/particle.vert", "shaders/particle.frag", nullptr, "particle");
	ResourceManager::loadShader("shaders/post_processing.vert", "shaders/post_processing.frag", nullptr, "postprocessing");


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
	ResourceManager::loadTexture("textures/powerup_speed.png", true, "powerup_speed");
	ResourceManager::loadTexture("textures/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::loadTexture("textures/powerup_passthrough.png", true, "powerup_passthrough");
	ResourceManager::loadTexture("textures/powerup_increase.png", true, "powerup_increase");
	ResourceManager::loadTexture("textures/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::loadTexture("textures/powerup_chaos.png", true, "powerup_chaos");

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
	Effects = new PostProcessor(ResourceManager::getShader("postprocessing"), m_width, m_height);
	Text = new TextRenderer(m_width, m_height);
	Text->load("fonts/OCRAEXT.TTF", 24);

	SoundEngine->setSoundVolume(soundVolume);
	SoundEngine->play2D("audio/breakout.mp3", true);
}

void Game::processInput(float dt) {
	
	float velocity = PLAYER_VELOCITY * dt;
	if (m_state == GAME_ACTIVE) {
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

	if (m_state == GAME_MENU) {

		if (this->m_keys[GLFW_KEY_ENTER] && !this->m_keysProcessed[GLFW_KEY_ENTER])
		{
			this->m_state = GAME_ACTIVE;
			this->m_keysProcessed[GLFW_KEY_ENTER] = true;
		}
		if (this->m_keys[GLFW_KEY_W] && !this->m_keysProcessed[GLFW_KEY_W])
		{
			this->m_level = (this->m_level + 1) % 4;
			this->m_keysProcessed[GLFW_KEY_W] = true;
		}
		if (this->m_keys[GLFW_KEY_S] && !this->m_keysProcessed[GLFW_KEY_S])
		{
			if (this->m_level > 0)
				--this->m_level;
			else
				this->m_level = 3;
			this->m_keysProcessed[GLFW_KEY_S] = true;
		}
	}
	if (m_state == GAME_WIN) {
		if (m_keys[GLFW_KEY_ENTER]) {
			m_keysProcessed[GLFW_KEY_ENTER] = true;
			Effects->chaos = false;
			m_state = GAME_MENU;
		}
	}
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

	m_lives = 3;
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

	updatePowerUps(dt);

	if (Ball->m_position.y >= m_height) {
		--m_lives;

		if (m_lives == 0) {
			resetLevel();
			m_state = GAME_MENU;
		}
		resetPlayer();
	}

	if (shakeTime >= 0.0f) {
		shakeTime -= dt;
		if (shakeTime <= 0.0f)
			Effects->shake = false;
	}

	if (m_state == GAME_ACTIVE && m_levels[m_level].isCompleted()) {
		resetLevel();
		resetPlayer();
		Effects->chaos = true;
		m_state = GAME_WIN;
	}
	
}

void Game::render() {

	if (m_state == GAME_ACTIVE || m_state == GAME_MENU) {
		Effects->beginRender();

		Renderer->drawSprite(ResourceManager::getTexture("background"), \
			glm::vec2(0.0f, 0.0f), glm::vec2(m_width, m_height), 0.0f);

		m_levels[m_level].draw(*Renderer);

		Particles->draw();

		Player->draw(*Renderer);

		for (PowerUp& powerUp : PowerUps) {
			if (!powerUp.m_destroyed)
				powerUp.draw(*Renderer);
		}

		Ball->draw(*Renderer);
		
		Effects->endRender();
		Effects->render(glfwGetTime());

		std::stringstream ss;
		ss << m_lives;
		Text->renderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (m_state == GAME_MENU) {
		Text->renderText("Press ENTER to start", 250.0f, m_height / 2, 1.0f);
		Text->renderText("Press W or S to select level", 245.0f, m_height / 2 + 20.0f, 0.75f);
	}
	if (m_state == GAME_WIN) {
		Text->renderText("you WON!", 320.0f, m_height / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->renderText("Press ENTER to retry or ESCAPE to quit", 130.0f, m_height / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}

}

bool isOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type) {
	for (const PowerUp& powerUp : powerUps) {
		if (powerUp.m_activated)
			if (powerUp.m_type == type)
				return true;
	}
	return false;
}

void Game::updatePowerUps(float dt) {
	for (PowerUp& powerUp : PowerUps) {
		powerUp.m_position += powerUp.m_velocity * dt;
		if (powerUp.m_activated) {
			powerUp.m_duration -= dt;

			if (powerUp.m_duration <= 0.0f) {
				powerUp.m_activated = false;

				if (powerUp.m_type == "sticky") {
					if (!isOtherPowerUpActive(PowerUps, "sticky")) {
						Ball->m_sticky = false;
						Player->m_color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.m_type == "pass-through") {
					if (!isOtherPowerUpActive(PowerUps, "pass-through")) {
						Ball->m_passThrough = false;
						Ball->m_color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.m_type == "confuse") {
					if (!isOtherPowerUpActive(PowerUps, "confuse")) {
						Effects->confuse = false;
					}
				}
				else if (powerUp.m_type == "chaos") {
					if (!isOtherPowerUpActive(PowerUps, "chaos")) {
						Effects->chaos = false;
					}
				}
			}
		}
	}
	PowerUps.erase(std::remove_if(PowerUps.begin(), PowerUps.end(), \
		[](const PowerUp& powerUp) {return powerUp.m_destroyed && !powerUp.m_activated; }), PowerUps.end());
}

bool shouldSpawn(unsigned int chance) {
	unsigned int random = rand() % chance;
	return random == 0;
}

void Game::spawnPowerUps(GameObject& block) {
	if (shouldSpawn(75))
		PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.m_position, ResourceManager::getTexture("powerup_speed")));
	if (shouldSpawn(75))
		PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.m_position, ResourceManager::getTexture("powerup_sticky")));
	if (shouldSpawn(75))
		PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.m_position, ResourceManager::getTexture("powerup_passthrough")));
	if (shouldSpawn(75))
		PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4f), 0.0f, block.m_position, ResourceManager::getTexture("powerup_increase")));
	if (shouldSpawn(15))
		PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.m_position, ResourceManager::getTexture("powerup_confuse")));
	if (shouldSpawn(15))
		PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.m_position, ResourceManager::getTexture("powerup_chaos")));
}

bool checkCollision(GameObject& one, GameObject& two);
Collision checkCollision(BallObject& one, GameObject& two);
Direction vectorDirection(glm::vec2 target);
void activatePowerUp(PowerUp& powerUp);


void Game::doCollision() {
	for (GameObject& box : m_levels[m_level].m_bricks) {

		if (!box.m_destroyed) {

			Collision collision = checkCollision(*Ball, box);

			if (std::get<0>(collision)) {

				if (!box.m_isSolid) {
					box.m_destroyed = true;
					spawnPowerUps(box);
					SoundEngine->play2D("audio/bleep.mp3", false);
				}
				else {
					shakeTime = 0.05f;
					Effects->shake = true;
					SoundEngine->setSoundVolume(1.0f);
					SoundEngine->play2D("audio/solid.wav", false);
					SoundEngine->setSoundVolume(soundVolume);
				}

				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);

				if (!(Ball->m_passThrough && !box.m_isSolid)) {

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

		Ball->m_stuck = Ball->m_sticky;
		SoundEngine->play2D("audio/bleep.wav", false);
	}

	for (PowerUp& powerUp : PowerUps) {

		if (!powerUp.m_destroyed) {

			if (powerUp.m_position.y >= m_height)
				powerUp.m_destroyed = true;

			if (checkCollision(*Player, powerUp)) {
				activatePowerUp(powerUp);
				powerUp.m_destroyed = true;
				powerUp.m_activated = true;
				SoundEngine->setSoundVolume(1.0f);
				SoundEngine->play2D("audio/powerup.wav", false);
				SoundEngine->setSoundVolume(soundVolume);
			}
		}
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

void activatePowerUp(PowerUp& powerUp) {
	if (powerUp.m_type == "speed") {
		Ball->m_velocity *= 1.2;
	}
	else if (powerUp.m_type == "sticky") {
		Ball->m_sticky = true;
		Player->m_color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.m_type == "pass-through") {
		Ball->m_passThrough = true;
		Ball->m_color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.m_type == "pad-size-increase") {
		Player->m_size.x += 50;
	}
	else if (powerUp.m_type == "confuse") {
		if (!Effects->chaos)
			Effects->confuse = true;
	}
	else if (powerUp.m_type == "chaos") {
		if (!Effects->confuse)
			Effects->chaos = true;
	}
}