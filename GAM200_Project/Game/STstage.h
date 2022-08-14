#pragma once
#include "../Engine/Scene.h"

class SkullTreeStage : public Scene {
public:
	void load() override;
	void unload() override;
	void update(double dt) override;
	void draw() override;

private:
	double bossDeadTimer = 0;
	bool hasPlayedFading = false;
	GameObject* clearFlagBlock = nullptr;

};

