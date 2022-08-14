#pragma once
#include "Button.h"
#include "../Engine/Scene.h"

class GameMap;
class MainMenu : public Scene {
public:
	void update(double dt) override;
	void draw() override;
	void load() override;
	void unload() override;
private:
	std::vector<Button> buttons;
	bool hasPlayedFading = false;

	int selectedIndex = 0;
};

