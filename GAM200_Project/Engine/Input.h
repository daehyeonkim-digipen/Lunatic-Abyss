#pragma once
#include "Math.h"
#include "Texture.h"
#include <vector>

class InputKey {
public:
	enum class Keyboard {
		None, Enter, Escape, Space, Left, Up, Right, Down, Shift,
		A, B, C, D, E, F, G, H, I, J,
		K, L, M, N, O, P, Q, R, S, T,
		U, V, W, X, Y, Z, Tilde, Ctrl, Tab,
		_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
		Count
	};

	InputKey(Keyboard button);
	bool is_key_down() const;
	bool is_key_released() const;
	bool is_key_pressed() const;
private:
	Keyboard button;
};

class InputMouse {
public:
	enum class MouseButton {
		None, Left, Right, Center, Count
	};

	InputMouse(MouseButton button);
	bool is_mouse_down() const;
	bool is_mouse_released() const;
	bool is_mouse_pressed();
private:
	MouseButton button;
};

class Input : public Component {
public:
	Input();
	bool is_key_down(InputKey::Keyboard key) const;
	bool is_key_released(InputKey::Keyboard key) const;
	bool is_key_pressed(InputKey::Keyboard key) const;
	void set_key_down(InputKey::Keyboard key, bool value);

	bool is_mouse_down(InputMouse::MouseButton button) const;
	bool is_mouse_released(InputMouse::MouseButton button) const;
	bool is_mouse_pressed(InputMouse::MouseButton button) const;
	void set_mouse_down(InputMouse::MouseButton button, bool value);

	void set_mouse_position(math::vec2 DEBUG_NEWPosition);
	void set_prev_mouse_position(math::vec2 DEBUG_NEWPosition);
	math::vec2 get_mouse_position() const;
	math::vec2 get_viewport_mouse_position() const;
	math::vec2 get_mouse_direction() const;
	void set_key_work(bool input);
	void update(double ) override;

private:
	std::vector<bool> keyDown;
	std::vector<bool> wasKeyDown;

	std::vector<bool> buttonDown;
	std::vector<bool> wasButtonDown;
	bool isKeyWork = true;
	math::vec2 mousePosition;
	math::vec2 previouseMousePosition;
};