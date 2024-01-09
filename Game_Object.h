#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Sprite_Renderer.h"

class GameObject {
public:
	glm::vec2 m_position, m_size, m_velocity;
	glm::vec3 m_color;
	float m_rotation;
	bool m_isSolid;
	bool m_destroyed;

	Texture m_sprite;

	GameObject(glm::vec2 pos = glm::vec2(0.0f, 0.0f), glm::vec2 size = glm::vec2(1.0f, 1.0f),\
		Texture sprite = Texture(), glm::vec3 color = glm::vec3(1.0f), \
		glm::vec2 velocity = glm::vec2(0.0f, 0.0f), float rotation = 0.0f, \
		bool isSolid = false, bool destroyed = false);

	virtual void draw(SpriteRenderer& renderer);
};



#endif