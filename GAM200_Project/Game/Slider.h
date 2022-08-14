#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Input.h"
#include "Button.h"
#include <map>

class Slider : public GameObject {
public:
	Slider();

	void update_slider(float& value);
	void draw(math::TransformMatrix viewportMatrix = {}) override;
	void load(math::vec2 pos, float& value_ref, float max_val);
	void unload();

private:
	struct Lever {
		Texture* texturePtr;
		math::vec2 position;
	} lever;

	Texture* sliderTex;
	InputMouse leftMouse;

	double length = 300.0;
	bool isPressed = false;
	bool hadPlayedSound = false;

	float maxValue = 100;
};

