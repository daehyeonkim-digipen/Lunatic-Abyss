#pragma once
#include "../Engine/Scene.h"

#include <vector>

class Texture;
class Splash : public Scene {
public:
	void update(double dt) override;
	void draw() override;
	void load() override;
	void unload() override;
private:
	double eachSplashTimer = 0;
	static constexpr double SPLASH_TIME = 3;
	static constexpr double FADING_TIME = 0.5;

	bool hasPlayedFading = false;

	std::vector<Texture*> splashes;
	int currSplashIndex = 0;
};

