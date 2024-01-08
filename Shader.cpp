#include "Shader.h"


Shader& Shader::use() {
	glUseProgram(this->ID);
	return *this;
}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {

	unsigned int vertex, fragment, geometry;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// geometry shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &geometrySource, NULL);
	glCompileShader(geometry);
	checkCompileErrors(geometry, "GEOMETRY");

	// shader program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	if (!geometrySource)	glAttachShader(this->ID, geometry);
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");

	// removing shader after linking with shader program
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!geometrySource)	glDeleteShader(geometry);

}


void Shader::setFloat(const char* name, float value, bool useShader) {
	if (useShader) this->use();
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::setInt(const char* name, int value, bool useShader) {
	if (useShader) this->use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::setVec(const char* name, const glm::vec2& value, bool useShader) {
	if (useShader) this->use();
	glUniform2fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(value));
}
void Shader::setVec(const char* name, const glm::vec3& value, bool useShader) {
	if (useShader) this->use();
	glUniform3fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(value));
}
void Shader::setVec(const char* name, const glm::vec4& value, bool useShader) {
	if (useShader) this->use();
	glUniform4fv(glGetUniformLocation(this->ID, name), 1, glm::value_ptr(value));
}
void Shader::setMat(const char* name, const glm::mat2& value, bool useShader) {
	if (useShader) this->use();
	glUniformMatrix2fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(value));
}
void Shader::setMat(const char* name, const glm::mat3& value, bool useShader) {
	if (useShader) this->use();
	glUniformMatrix3fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(value));
}
void Shader::setMat(const char* name, const glm::mat4& value, bool useShader) {
	if (useShader) this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(value));
}

void Shader::checkCompileErrors(unsigned int object, std::string type) {
	int success;
	char infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cerr << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cerr << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}