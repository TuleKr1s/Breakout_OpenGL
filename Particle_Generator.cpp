#include "Particle_Generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount)
	: m_shader(shader), m_texture(texture), m_amount(amount) 
{
	init();
}

void ParticleGenerator::update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset) {

	for (unsigned int i = 0; i < newParticles; ++i) {
		int unusedParticle = firstUnusedParticle();
		respawnParticle(particles[unusedParticle], object, offset);
	}

	for (unsigned int i = 0; i < m_amount; ++i) {
		Particle& p = particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f) {
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5f;
		}
	}

}


void ParticleGenerator::draw() {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_shader.use();
	for (Particle particle : particles) {
		if (particle.Life > 0.0f) {
			m_shader.setVec("offset", particle.Position);
			m_shader.setVec("color", particle.Color);
			m_texture.bind();
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < m_amount; ++i)
		particles.push_back(Particle());
	
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle() {
	for (unsigned int i = lastUsedParticle; i < m_amount; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset) {


	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = object.m_position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.m_velocity * 0.1f;
}