#include "Font.h"

#include "TextureManager.h"

Font::Font(const std::string& fileName) : texture(Engine::get_GE_component<TextureManager>()->load(fileName)) {
	unsigned int white = 0xFFFFFFFF;

	// The top right pixel MUST be white (0xFFFFFFFF)
	unsigned int color = texture->get_pixel( 0, (int)texture->get_size().y-1);
	if (color != white) {
		Engine::get_logger().log_error("Font " + fileName + " not setup correctly");
		return;
	}

	// Build our rects
	setup_char_rects();
}

void Font::draw_text_to_texture(math::TransformMatrix positionMatrix, std::string text_)
{
	for (char c : text_) {
		draw_char(positionMatrix, c);
	}
}

void Font::setup_char_rects()
{
	int height = static_cast<int>(texture->get_size().y - 1);
	unsigned int testColor = texture->get_pixel(0, height);
	unsigned int nextColor;

	int xPos = 0;
	for (int index = 0; index < Font::numOfChars; index++) {
		int width = 0;
		do {
			width++;
			nextColor = texture->get_pixel(xPos + width, height);
		} while (testColor == nextColor);
		testColor = nextColor;
		charTexels[index].point2 = math::vec2(xPos + width - 1, 0 );
		charTexels[index].point1 = math::vec2(static_cast<double>(xPos), charTexels[index].point1.y + height - 1);
		xPos += width;
	}
}

math::rect2& Font::get_char_rect(char c)
{
	if (c >= ' ' && c <= 'z') {
		return charTexels[c - ' '];
	}
	else {
		Engine::get_logger().log_error("character '" + std::to_string(c) + "' not found in spriteFont");
		return charTexels[0];
	}
}

math::vec2 Font::measure_text(std::string text)
{
	math::vec2 size = { 0,0 };
	for (char c : text) {
		math::rect2 charRect = get_char_rect(c);
		size.x += get_char_rect(c).Size().x;
		if (charRect.Size().y > size.y) {
			size.y = charRect.Size().y;
		}
	}
	return size;
}

void Font::draw_char(math::TransformMatrix& matrix, char c)
{
	math::rect2& displayRect = get_char_rect(c);
	math::vec2 topLeftTexel = { displayRect.point1.x, displayRect.point2.y };
	if (c != ' ') {
		texture->draw(matrix, topLeftTexel, displayRect.Size());
	}
	matrix *= math::TranslateMatrix(math::vec2{ displayRect.Size().x, 0. });
}
