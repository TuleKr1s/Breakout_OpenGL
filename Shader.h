#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>


class Shader {
public:

	unsigned int ID;

	Shader() {}

	Shader& use();

	void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);


	void setFloat(const char* name, float value, bool useShader = false);
	void setInt(const char* name, int value, bool useShader = false);

	// overloaded function
	// to set vector to uniform variable
	void setVec(const char* name, const glm::vec2& value, bool useShader = false);
	void setVec(const char* name, const glm::vec3& value, bool useShader = false);
	void setVec(const char* name, const glm::vec4& value, bool useShader = false);

	// to set matrix to uniform variable 
	void setMat(const char* name, const glm::mat2& value, bool useShader = false);
	void setMat(const char* name, const glm::mat3& value, bool useShader = false);
	void setMat(const char* name, const glm::mat4& value, bool useShader = false);
	
private:
	void checkCompileErrors(unsigned int object, std::string type);

};



#endif