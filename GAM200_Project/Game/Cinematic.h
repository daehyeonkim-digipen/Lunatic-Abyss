#pragma once
#include "../Engine/Component.h"

class Texture;
class Cinematic : public Component
{
public:
	Cinematic();
	~Cinematic() override;

	void update(double dt) override;
	void draw();
	bool is_playing() { return isCinematic; };

	void stop();
	void play(double time = 600);
private:
	Texture* blackLineTex = nullptr;

	double lineTimer = 0;
	double playTime = 0;
	
	static constexpr double lineHeight = 130;
	static constexpr double lineSpeedOffset = 400;

	bool isCinematic = false;
};

