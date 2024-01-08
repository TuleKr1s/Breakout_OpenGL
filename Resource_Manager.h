#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


#include <glad/glad.h>

#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "stb_image.h"

#include "Texture.h"
#include "Shader.h"

class ResourceManager {
public:

	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;

	static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static Shader getShader(std::string name);

	static Texture loadTexture(const char* file, bool alpa, std::string name);
	static Texture getTexture(std::string name);

	static void clear();

private:

	ResourceManager() {}


	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);

	static Texture loadTextureFromFile(const char* file, bool alpha);

};



#endif