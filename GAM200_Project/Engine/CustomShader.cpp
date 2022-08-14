#include "Engine.h"
#include "Graphic.h"

#include "CustomShader.h"

#include <iostream>
#include <sstream>

CustomShader::CustomShader(std::string vertFile, std::string fragFile, std::string geomFile , std::string tescFile , std::string teseFile) {
	if (vertFile.empty() == false)
		compileFile(GL_VERTEX_SHADER, vertFile);

	if (fragFile.empty() == false)
		compileFile(GL_FRAGMENT_SHADER, fragFile);

	if (geomFile.empty() == false)
		compileFile(GL_GEOMETRY_SHADER, geomFile);

	if (tescFile.empty() == false && teseFile.empty() == false) {
		compileFile(GL_TESS_CONTROL_SHADER, tescFile);
		compileFile(GL_TESS_EVALUATION_SHADER, teseFile);
		doesUsingTessellation = true;
	}

	link_shaders();
}

void CustomShader::enable() {
	glUseProgram(shaderID);
}

void CustomShader::disable() {
	glUseProgram(0);
}

void CustomShader::remove() {
	if (shaderID > 0)
		glDeleteProgram(shaderID);
}

void CustomShader::compileFile(unsigned int shader_type, const std::string& file_name) {
	std::ifstream shader_file;
	std::string shader_source_str;

	shader_file.open(file_name);
	assert(shader_file.good());

	if (shaderID <= 0) {
		shaderID = glCreateProgram();
		bool shader_program_failed = shaderID > 0;
		assert(shader_program_failed);

	}

	std::stringstream shader_stream;
	shader_stream << shader_file.rdbuf();

	shader_file.close();

	shader_source_str = shader_stream.str();
	compileString(shader_type, shader_source_str.c_str());
}

void CustomShader::compileString(unsigned int shader_type, const std::string& shader_src) {
	if (shaderID <= 0) {
		shaderID = glCreateProgram();
	}

	unsigned int shader_handle;
	shader_handle = glCreateShader(shader_type);

	const char* shader_source_c_str[] = { shader_src.c_str() };
	glShaderSource(shader_handle, 1, shader_source_c_str, NULL);

	glCompileShader(shader_handle);

	int success;	char infoLog[512];
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader_handle, 512, NULL, infoLog);
		Engine::get_logger().log_debug(infoLog);
	}

	glAttachShader(shaderID, shader_handle);
}

void CustomShader::link_shaders() {
	if (shaderID <= 0) {
		Engine::get_logger().log_debug("undefined shader program ID. not linked");
		return;
	}

	glLinkProgram(shaderID);

	int success;	char infoLog[512];
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
		Engine::get_logger().log_debug(infoLog);
	}
}

void CustomShader::setUniform(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}

void CustomShader::setUniform(const std::string& name, unsigned int value) const {
	Engine::get_logger().log_debug("Uniform " + name + " location : " + std::to_string(glGetUniformLocation(shaderID, name.c_str())) + " " + std::to_string(value));
	glUniform1ui(glGetUniformLocation(shaderID, name.c_str()), value);
}

void CustomShader::setUniform(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void CustomShader::setUniform(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void CustomShader::setUniform(const std::string& name, double value) const {
	glUniform1d(glGetUniformLocation(shaderID, name.c_str()), value);
}

void CustomShader::setUniform(const std::string& name, math::vec2 vec2) const {
	glUniform2d(glGetUniformLocation(shaderID, name.c_str()), vec2.x, vec2.y);
}

void CustomShader::setUniform(const std::string& name, glm::vec2 vec2) const {
	glUniform2f(glGetUniformLocation(shaderID, name.c_str()), vec2.x, vec2.y);
}

void CustomShader::setUniform(const std::string& name, glm::vec3 vec3) const {
	glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vec3.r, vec3.g, vec3.b);
}

void CustomShader::setUniform(const std::string& name, glm::vec4 vec4) const {
	glUniform4f(glGetUniformLocation(shaderID, name.c_str()), vec4.r, vec4.g, vec4.b, vec4.a);
}

void CustomShader::setUniform(const std::string& name, glm::mat3 mat3) const {
	//Engine::get_logger().log_debug("Uniform " + name + " location : " + std::to_string(glGetUniformLocation(shaderID, name.c_str())));
	glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat3[0][0]);
}

void CustomShader::setUniform(const std::string& name, glm::mat4 mat4) const {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat4[0][0]);
}