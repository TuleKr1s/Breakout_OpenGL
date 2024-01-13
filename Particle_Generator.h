#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Game_Object.h"

#include <vector>

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}

};

class ParticleGenerator {
public:
	ParticleGenerator(Shader shader, Texture texture, unsigned int amount);

	void update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

	void draw();

private:
	std::vector<Particle> particles;
	unsigned int m_amount;

	Shader m_shader;
	Texture m_texture;
	unsigned int VAO;

	void init();

	unsigned int firstUnusedParticle();

	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};


#endif
