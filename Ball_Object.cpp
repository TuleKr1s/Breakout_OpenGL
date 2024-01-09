#include "Ball_Object.h"

BallObject::BallObject()
	: GameObject(), m_radius(12.5f), m_stuck(true) {}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite)
	: GameObject(pos, glm::vec2(radius*2.0f, radius*2.0f), sprite, glm::vec3(1.0f), velocity), m_radius(radius), m_stuck(true) {}

glm::vec2 BallObject::move(float dt, unsigned int windowWidth) {

	if (!m_stuck) {
		m_position += m_velocity * dt;

		if (m_position.x <= 0.0f) {
			m_velocity.x = -m_velocity.x;
			m_position.x = 0.0f;
		}
		else if (m_position.x + m_size.x >= windowWidth) {
			m_velocity.x = -m_velocity.x;
			m_position.x = windowWidth - m_size.x;
		}
		if (m_position.y <= 0.0f) {
			m_velocity.y = -m_velocity.y;
			m_position.y = 0.0f;
		}
		// test
		/*else if (m_position.y + m_size.y >= 600) {
			m_velocity.y = -m_velocity.y;
			m_position.y = 600 - m_size.y;
		}*/
	}

	return m_position;
}


void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {
	m_position = position;
	m_velocity = velocity;
	m_stuck = true;
}