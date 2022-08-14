#pragma once
#include <string>
#include "Texture.h"
#include "Component.h"

class Font : public Component
{
public:
	Font(const std::string& fileName);
	void draw_text_to_texture(math::TransformMatrix positionMatrix, std::string text);
	math::vec2 measure_text(std::string text);

	math::rect2& get_char_rect(char c);
private:
	void setup_char_rects();
	void draw_char(math::TransformMatrix& matrix, char c);

	Texture* texture;
	static constexpr int numOfChars = 'z' - ' ' + 1;
	math::rect2 charTexels[numOfChars];
};