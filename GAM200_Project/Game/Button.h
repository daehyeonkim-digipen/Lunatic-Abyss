#pragma once
#include <string>
#include "../Engine/GameObject.h"

class Texture;
class Button : public GameObject
{
public:
	Button();
	bool is_button_pressed();
	bool is_button_released();
	bool is_mouse_on_button();
	math::vec2 get_button_size();
	void update(double dt) override;
	void draw(math::TransformMatrix displayMt = {}) override;
	void load(math::vec2 initPos, std::string buttonImage);
	void unload();
private:
	Texture* texture = nullptr;
	math::vec2 size;
	bool isClicked;
	bool isReleased;
};