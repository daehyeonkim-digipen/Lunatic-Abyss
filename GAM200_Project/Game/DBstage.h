#pragma once
#include "../Engine/Scene.h"

class DarkBoar;

class DarkBoarStage : public Scene {
public:
	void load() override;
	void unload() override;
	void update(double dt) override;
	void draw() override;

private:
	GameObject* clearFlagBlock = nullptr;

	double bossDeadTimer = 0;
	bool hasPlayedFading = false;

};

