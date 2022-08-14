#pragma once
#include "../Engine/Component.h"
#include "../Engine/Input.h"

class Cheat : public Component {
	typedef InputKey::Keyboard GameKey;

public:
	Cheat(GameKey key);

	void update(double dt) override;
private:
	InputKey CheatKey;
};

