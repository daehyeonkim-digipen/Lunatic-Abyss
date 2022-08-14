#pragma once
#include "Input.h"
#include "Component.h"

class ShowCollision : public Component {
public:
	ShowCollision(InputKey::Keyboard key_);
	void update(double dt) override;
	bool is_enabled();
private:
	bool isEnabled; 
	InputKey key;
};

