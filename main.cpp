#include <glad/glad.h> // necessarily before others
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Game.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

Game Breakout(SCR_WIDTH, SCR_HEIGHT);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set icon
	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/icon.png", &width, &height, &nrChannels, 0);
	if (data) {
		GLFWimage icon;
		icon.width = width;
		icon.height = height;
		icon.pixels = data;

		glfwSetWindowIcon(window, 1, &icon);

		stbi_image_free(data);
	}
	else {
		std::cerr << "Failed to load icon" << std::endl;
	}

	Breakout.init();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		Breakout.processInput(deltaTime);
		Breakout.update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.render();

		glfwSwapBuffers(window);
	}

	ResourceManager::clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.m_keys[key] = true;
		else if (action == GLFW_RELEASE)
			Breakout.m_keys[key] = false;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}