#pragma once
#include "Component.h"
#include "Graphic.h"
#include "CustomShader.h"
#include "Math.h"

class ShaderEffect : public Component {
	friend class GameObject;
public:
	ShaderEffect() {};
	ShaderEffect(CustomShader displayShader, math::vec2 size_ = { 100, 100 });
	void remove_shader();

	void draw(math::TransformMatrix displayMt);

	template <typename T>
	void set_variable(std::string variableName, T typeValue);

private:
	IGL::Graphic::Model model;
	CustomShader effectShader;

	math::vec2 size;
};

template<typename T>
inline void ShaderEffect::set_variable(std::string variableName, T typeValue) {
	effectShader.enable();
	effectShader.setUniform(variableName, typeValue);
	effectShader.disable();
}