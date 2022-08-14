#include "Engine.h"	
#include "Graphic.h"
#include "Input.h"

Input::Input() {
	keyDown.resize(static_cast<int>(InputKey::Keyboard::Count));
	wasKeyDown.resize(static_cast<int>(InputKey::Keyboard::Count));
	buttonDown.resize(static_cast<int>(InputMouse::MouseButton::Count));
	wasButtonDown.resize(static_cast<int>(InputMouse::MouseButton::Count));
	// Engine::get_logger().log_debug("Input inserted\n");
}

bool Input::is_key_down(InputKey::Keyboard key) const {
	return isKeyWork&& keyDown[static_cast<int>(key)];
}

bool Input::is_key_released(InputKey::Keyboard key) const {
	return isKeyWork && keyDown[static_cast<int>(key)] == false && wasKeyDown[static_cast<int>(key)] == true;
}

bool Input::is_key_pressed(InputKey::Keyboard key) const {
	return isKeyWork && keyDown[static_cast<int>(key)] == true && wasKeyDown[static_cast<int>(key)] == false;
}

void Input::set_key_down(InputKey::Keyboard key, bool value) {
	keyDown[static_cast<int>(key)] = value;
}


void Input::set_mouse_position(math::vec2 newPosition) {
	mousePosition = newPosition;
}

void Input::set_prev_mouse_position(math::vec2 newPosition) {
	previouseMousePosition = newPosition;
}

math::vec2 Input::get_mouse_position() const {
	return mousePosition;
}

math::vec2 Input::get_viewport_mouse_position() const {
	return mousePosition / IGL::Graphic::VIEWPORT_RATIO;
}

math::vec2 Input::get_mouse_direction() const {
	return mousePosition - previouseMousePosition;
}

void Input::set_key_work(bool input)
{
	isKeyWork = input;
}

bool Input::is_mouse_down(InputMouse::MouseButton button) const {
	return isKeyWork && buttonDown[static_cast<int>(button)];
}

bool Input::is_mouse_released(InputMouse::MouseButton button) const {
	return isKeyWork && buttonDown[static_cast<int>(button)] == false && wasButtonDown[static_cast<int>(button)] == true;
}

bool Input::is_mouse_pressed(InputMouse::MouseButton button) const {
	return isKeyWork && buttonDown[static_cast<int>(button)] == true && wasButtonDown[static_cast<int>(button)] == false;
}

void Input::set_mouse_down(InputMouse::MouseButton button, bool value) {
	buttonDown[static_cast<int>(button)] = value;
}


void Input::update(double)
{
	wasKeyDown = keyDown;
	wasButtonDown = buttonDown;
}

InputKey::InputKey(InputKey::Keyboard button) : button(button) {}

bool InputKey::is_key_down() const {
	return Engine::get_GE_component<Input>()->is_key_down(button);
}

bool InputKey::is_key_released() const {
	return Engine::get_GE_component<Input>()->is_key_released(button);
}

bool InputKey::is_key_pressed() const {
	return Engine::get_GE_component<Input>()->is_key_pressed(button);
}

InputMouse::InputMouse(InputMouse::MouseButton button) : button(button) {}

bool InputMouse::is_mouse_down() const {
	return Engine::get_GE_component<Input>()->is_mouse_down(button);
}

bool InputMouse::is_mouse_released() const {
	return Engine::get_GE_component<Input>()->is_mouse_released(button);
}

bool InputMouse::is_mouse_pressed() {
	return Engine::get_GE_component<Input>()->is_mouse_pressed(button);
}