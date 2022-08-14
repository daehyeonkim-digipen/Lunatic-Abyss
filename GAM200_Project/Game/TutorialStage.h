#pragma once
#include "../Engine/Scene.h"
#include <vector>
#include <string>

class TutorialStage : public Scene {
public:
	TutorialStage();

	void load() override;
	void unload() override;
	void update(double dt) override;
	void draw() override;
private:
	void hint_cinematic_play();

	double elapsedTime;
	int hintIndex;
	std::vector<std::string> hints;

	std::string reviewInfo;
	InputKey hintReviewKey;
	InputKey hintSkipKey;

};

