#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Sprite_Renderer.h"
#include "Shader.h"

#include <iostream>

class PostProcessor {
public:
	Shader PostProcessingShader;
	Texture m_texture;
	unsigned int m_width, m_height;

	bool confuse, chaos, shake;

	PostProcessor(Shader shader, unsigned int width, unsigned int height);

	void beginRender();
	void endRender();

	void render(float time);

private:
	unsigned int MSFBO, FBO;
	unsigned int RBO;
	unsigned int VAO;

	void initRenderData();
};


#endif