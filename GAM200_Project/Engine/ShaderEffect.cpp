#include "shaderEffect.h"

ShaderEffect::ShaderEffect(CustomShader displayShader, math::vec2 size_)
	: effectShader(displayShader), size(size_) {
	model = Engine::get_GE_component<IGL::Graphic>()->create_model();
}

void ShaderEffect::remove_shader() {
	effectShader.remove();
}

void ShaderEffect::draw(math::TransformMatrix displayMt) {
	effectShader.enable();
	glBindVertexArray(model.VAO_ID);
	
	// ===========================================================================================================
	math::vec2 winSize = Engine::get_GE_component<IGL::Graphic>()->get_window_size();
	glm::mat3 to_ndc(2.f * displayMt[0][0] / winSize.x, 2.f * displayMt[1][0] / winSize.y, 0, 2.f * displayMt[0][1] / winSize.x, 2.f * displayMt[1][1] / winSize.y, 0, 2.f * displayMt[0][2] / winSize.x - 1.f, 2.f * displayMt[1][2] / winSize.y - 1.f, 1);

	effectShader.setUniform(Uniforms::NDC, to_ndc);
	// ===========================================================================================================

	effectShader.setUniform(Uniforms::ElapsedTime, Engine::ElpasedTime);
	effectShader.setUniform(Uniforms::Resolution, winSize);

	//// ===========================================================================================================
	glm::mat3 modelMt(size.x, 0, 0,    0, size.y, 0,    0, 0, 1);

	effectShader.setUniform(Uniforms::Model, modelMt);
	//// ===========================================================================================================


	glDrawElements(model.primitive_type, model.draw_count, GL_UNSIGNED_SHORT, nullptr);

	glBindVertexArray(0);
	effectShader.disable();
}
