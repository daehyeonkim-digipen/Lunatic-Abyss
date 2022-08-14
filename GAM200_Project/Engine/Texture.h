#pragma once
#include <glm/glm.hpp>

#include "Math.h"
#include "Component.h"
#include "Graphic.h"

#include "CustomShader.h"

// model for texture should be square

class Texture : public Component {
	friend class TextureManager;
private:
	IGL::Graphic::Model model;
	CustomShader textureShader;

	int width = 0;
	int height = 0;
	unsigned int byteSize = 0;
	unsigned int handle = 0;

	int unitID = 0;
	bool damagedTexture = false;
	unsigned char* imageInfo = nullptr;

	glm::vec4 uColor{ 1, 1, 1, 1 };

public:
	Texture() {}
	Texture(const std::string& path, unsigned int textureUnitID);
	~Texture();
	void set_tint(double r, double g, double b, double a = 1.0);
	void set_shader(const std::string&, const std::string&);
	math::vec2 get_size();

	unsigned int get_pixel(int row, int column);
	void bind_color();

	void draw(math::TransformMatrix displayMatrix, math::vec2 texelPos, math::vec2 frameSize);
	void draw(math::TransformMatrix displayMatrix);

	void set_damaged(bool);
};
