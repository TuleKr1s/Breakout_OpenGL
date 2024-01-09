#ifndef BALL_OBJECT_H
#define BALL_OBJECT_H

#include "Game_Object.h"

class BallObject : public GameObject {
public:
	
	float m_radius;
	bool m_stuck;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture sprite);

	glm::vec2 move(float dt, unsigned int widnowWidth);
	void reset(glm::vec2 position, glm::vec2 velocity);
};



#endif