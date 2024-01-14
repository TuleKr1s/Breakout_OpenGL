#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Resource_Manager.h"
#include "Texture.h"
#include "Shader.h"

#include <map>

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer {
public:

	std::map<char, Character> Characters;

	Shader TextShader;

	TextRenderer(unsigned int width, unsigned int height);

	void load(std::string font, unsigned int fontSize);

	void renderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:

	unsigned int VAO, VBO;

};


#endif