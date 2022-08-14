#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Uniforms {
	const std::string Texture2D			= "u_texture";
	const std::string NDC				= "u_NDC";
	const std::string Model				= "u_model";

	const std::string ElapsedTime		= "u_time";
	const std::string Resolution		= "u_resolution";

	const std::string DustAlpha			= "u_dustAlpha";
	const std::string Color				= "u_color";
	const std::string LifeTime			= "u_lifeTime";
}

class CustomShader {
public:
	CustomShader() {};
	CustomShader(std::string vertFile, std::string fragFile, std::string geomFile = "", std::string tescFile = "", std::string teseFile = "");

	void enable();
	void disable();
	void remove();

	void setUniform(const std::string& name, bool value) const;
	void setUniform(const std::string& name, unsigned int value) const;
	void setUniform(const std::string& name, int value) const;
	void setUniform(const std::string& name, float value) const;
	void setUniform(const std::string& name, double value) const;
	void setUniform(const std::string& name, math::vec2 vec2) const;
	void setUniform(const std::string& name, glm::vec2 vec2) const;
	void setUniform(const std::string& name, glm::vec3 vec3) const;
	void setUniform(const std::string& name, glm::vec4 vec4) const;
	void setUniform(const std::string& name, glm::mat3 mat3) const;
	void setUniform(const std::string& name, glm::mat4 mat4) const;

private:
	int shaderID = 0;
	bool doesUsingTessellation = false;
	
	void compileFile(unsigned int shader_type, const std::string& file_name);
	void compileString(unsigned int shader_type, const std::string& shader_src);
	void link_shaders();
};

