#include "../Engine/Engine.h"
#include "../Engine/Input.h"
#include "../Engine/Texture.h"
#include "../Engine/Graphic.h"
#include "../Engine/Audio.h"
#include "../Engine/TextureManager.h"
#include "Slider.h"

Slider::Slider() : leftMouse(InputMouse::MouseButton::Left), sliderTex(nullptr) {}

void Slider::load(math::vec2 pos, float& value_ref, float max_val) {
	set_position(pos);

	double ratio = value_ref / max_val;
	lever.position = pos + math::vec2{ length * ratio, 0.0 };

	sliderTex = Engine::get_GE_component<TextureManager>()->load("assets/UI/Slider/pad.png");
	lever.texturePtr = Engine::get_GE_component<TextureManager>()->load("assets/UI/Slider/drag.png");

	hadPlayedSound = false;
}

void Slider::unload() {
	sliderTex = Engine::get_GE_component<TextureManager>()->load("assets/UI/Slider/pad.png");
	lever.texturePtr = Engine::get_GE_component<TextureManager>()->load("assets/UI/Slider/drag.png"); // calling back

	Engine::get_GE_component<TextureManager>()->remove_texture(sliderTex);
	Engine::get_GE_component<TextureManager>()->remove_texture(lever.texturePtr); // then delete it		to prevent double deleting
}

void Slider::update_slider(float& value) {
	math::vec2 mousePos = Engine::get_GE_component<Input>()->get_mouse_position();
	math::vec2 leverTexPos{ lever.position.x - lever.texturePtr->get_size().x / 2, lever.position.y };
	bool isMouseButtonDown = leftMouse.is_mouse_down();

	if (leverTexPos.x < mousePos.x && mousePos.x < leverTexPos.x + lever.texturePtr->get_size().x
		&& leverTexPos.y < mousePos.y && mousePos.y < leverTexPos.y + lever.texturePtr->get_size().y && isMouseButtonDown) {
		isPressed = true;

		if (hadPlayedSound == false) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
			hadPlayedSound = true;
		}
	}
	

	if (get_position().x < mousePos.x && mousePos.x < get_position().x + sliderTex->get_size().x
		&& lever.position.y < mousePos.y && mousePos.y < lever.position.y + lever.texturePtr->get_size().y
		&& leftMouse.is_mouse_pressed()) {
		lever.position.x = mousePos.x;

		double sliderValue = (lever.position.x - get_position().x) / length;
		value = static_cast<float>(sliderValue * maxValue);
		// Engine::get_logger().log_debug(std::to_string(Engine::get_GE_components<Audio>()->get_bgm_sound()));
	}
	else if (isMouseButtonDown && isPressed) {
		 lever.position += math::vec2{ Engine::get_GE_component<Input>()->get_mouse_direction().x, 0.0 };
		 lever.position.x = std::clamp(lever.position.x, get_position().x, get_position().x + length);

		 double sliderValue = (lever.position.x - get_position().x) / length;
		 value = static_cast<float>(sliderValue * maxValue);
		 // Engine::get_logger().log_debug(std::to_string(Engine::get_GE_components<Audio>()->get_bgm_sound()));
	 }

	if (leftMouse.is_mouse_released()) {
		isPressed = false;
		hadPlayedSound = false;
	}
}

void Slider::draw(math::TransformMatrix ) {
	sliderTex->draw(get_matrix() * math::ScaleMatrix{ math::vec2{ length / sliderTex->get_size().x , 1.0 } });
	lever.texturePtr->draw(math::TranslateMatrix{ math::vec2{ lever.position.x - lever.texturePtr->get_size().x / 2, lever.position.y }});
}
