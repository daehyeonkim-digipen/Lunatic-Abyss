#pragma once
#pragma once
#include "../Engine/Scene.h"

class ShaderEffect;

class AWStage : public Scene {
private:
	std::vector<math::vec2> blockPos;
public:
	void load() override;
	void unload() override;
	void update(double dt) override;
	void draw() override;

private:
	double bossDeadTimer = 0;
	bool hasPlayedFading = false;
};

