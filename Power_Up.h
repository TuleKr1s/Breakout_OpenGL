#ifndef POWER_UP_H
#define POWER_UP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Game_Object.h"

#include <string>

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f); // power up velocity

class PowerUp : public GameObject {
public:
	std::string m_type;
	float m_duration;
	bool m_activated;

	PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture texture)
		: GameObject(position, POWERUP_SIZE, texture, color, VELOCITY), m_type(type), m_duration(duration), m_activated() {}
};




#endif