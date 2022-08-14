#pragma once
#include "../Engine/Scene.h"

class FAStage : public Scene {
public:
	void load() override;
	void unload() override;
	void update(double dt) override;
	void draw() override;

private:
	double gimmickTimer = 1.2;
	GameObject* light;

	bool lighton = false;
	bool hasInitHUD = false;

	double bossDeadTimer = 0;
	bool hasPlayerFading = false;
};

