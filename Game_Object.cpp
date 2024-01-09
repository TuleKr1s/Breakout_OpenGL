#include "Game_Object.h"


GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color, \
	glm::vec2 velocity, float rotation, bool isSolid, bool destroyed) 
	: m_position(pos), m_size(size), m_sprite(sprite), m_color(color), m_velocity(velocity),\
	m_rotation(rotation), m_isSolid(isSolid), m_destroyed(destroyed) 
{

}

void GameObject::draw(SpriteRenderer& renderer) {
	renderer.drawSprite(m_sprite, m_position, m_size, m_rotation, m_color);
}