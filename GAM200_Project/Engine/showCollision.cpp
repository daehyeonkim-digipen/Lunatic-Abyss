#include "ShowCollision.h"

ShowCollision::ShowCollision(InputKey::Keyboard key_)
	: key(key_), isEnabled(false)
{}

void ShowCollision::update(double) {
	if (key.is_key_pressed()) {
		isEnabled = !isEnabled;
	}
}

bool ShowCollision::is_enabled() {
#ifdef _DEBUG
	return isEnabled;
#else
	return false;
#endif
}


