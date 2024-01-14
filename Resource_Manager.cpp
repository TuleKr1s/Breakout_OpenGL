#include "Resource_Manager.h"

std::map<std::string, Texture> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name) {
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::getShader(std::string name) {
	if (Shaders.find(name) == Shaders.end())
		std::cerr << "Failed to get Shader: Unknonw name of shader \"" << name << "\"" << std::endl;
	return Shaders[name];
}

Texture ResourceManager::loadTexture(const char* file, bool alpha, std::string name) {
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture ResourceManager::getTexture(std::string name) {
	if (Textures.find(name) == Textures.end())
		std::cerr << "Failed to get Texture: Unknown name of texture \"" << name << "\"" << std::endl;
	return Textures[name];
}

void ResourceManager::clear() {
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);

	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try {

		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;

		if (!vertexShaderFile.is_open())
			std::cerr << "Failed to load vertex shader file" << std::endl;
		if (!fragmentShaderFile.is_open())
			std::cerr << "Failed to load fragment shader file" << std::endl;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderFile != nullptr) {
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e) {
		std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
	Texture texture;
	if (alpha) {
		texture.m_internalFormat = GL_RGBA;
		texture.m_imageFormat = GL_RGBA;
	}


	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data) {
		texture.generate(width, height, data);
	}
	else {
		std::cerr << "Failed to load texture: " << file << std::endl;
	}

	stbi_image_free(data);
	return texture;
}