#include <SFML/Graphics/Image.hpp>

#include "Texture.h"

Texture::Texture(const std::string& path, unsigned int textureUnitID) : unitID(textureUnitID) {
	sf::Image image;
	if(!image.loadFromFile(path.c_str())) {
		Engine::get_logger().log_error("Failed to load image");
	}

	image.flipVertically();
	width = image.getSize().x;
	height = image.getSize().y;
	byteSize = width * height * sizeof(int);
	imageInfo = new unsigned char[byteSize];
	memcpy(imageInfo, image.getPixelsPtr(), byteSize);

	bind_color();

	model = Engine::get_GE_component<IGL::Graphic>()->create_model();
	textureShader = CustomShader("assets/Shaders/IGL.vert", "assets/Shaders/IGL.frag");
}

Texture::~Texture() {
	textureShader.remove();
}

void Texture::set_tint(double r, double g, double b, double a) {
	uColor = { (float)r, (float)g, (float)b, (float)a };
}

void Texture::set_shader(const std::string& vpath, const std::string& fpath)
{
	textureShader = CustomShader(vpath, fpath);
}

math::vec2 Texture::get_size() {
	return { (double)width, (double)height };
}

unsigned int Texture::get_pixel(int row, int column) {
	int loc = row + column * width;
	return *(reinterpret_cast<unsigned int*>(imageInfo) + loc);
}

void Texture::bind_color() {
	glCreateTextures(GL_TEXTURE_2D, 1, &handle);
	glTextureStorage2D(handle, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageInfo);
	glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTextureUnit(unitID, handle);
}

void Texture::draw(math::TransformMatrix matrix, math::vec2 texelPos, math::vec2 frameSize) {
	textureShader.enable();
	glBindVertexArray(model.VAO_ID);

	textureShader.setUniform(Uniforms::Texture2D, unitID);

	// ===========================================================================================================
	const int windowW = Engine::get_GE_component<IGL::Graphic>()->windowWidth;
	const int windowH = Engine::get_GE_component<IGL::Graphic>()->windowHeight;
	glm::mat3 to_ndc(2.f * frameSize.x * matrix[0][0] / windowW, 2.f * frameSize.y * matrix[1][0] / windowH, 0, 2.f * frameSize.x * matrix[0][1] / windowW, 2.f * frameSize.y * matrix[1][1] / windowH, 0, 2.f * matrix[0][2] / windowW - 1.f, 2.f * matrix[1][2] / windowH - 1.f, 1);

	textureShader.setUniform(Uniforms::NDC, to_ndc); // shrink it
	// ===========================================================================================================

	textureShader.setUniform(Uniforms::Color, uColor); // color

	// ===========================================================================================================
	glm::mat3 texel(frameSize.x / width, 0, 0, 0, frameSize.y / height, 0, texelPos.x / width, texelPos.y / height, 1);

	textureShader.setUniform(Uniforms::Model, texel); // re-sizing as desired size
	// ===========================================================================================================

	glDrawElements(model.primitive_type, model.draw_count, GL_UNSIGNED_SHORT, nullptr);
	
	glBindVertexArray(0);
	textureShader.disable();
}

void Texture::draw(math::TransformMatrix displayMatrix) {
	draw(displayMatrix, { 0, 0 }, get_size());
}

void Texture::set_damaged(bool input) {
	damagedTexture = input;
}
