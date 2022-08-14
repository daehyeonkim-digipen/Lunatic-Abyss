#pragma once
#include <string>
#include "..\Engine\Scene.h"
#include "..\Engine\Texture.h"
#include "..\Engine\Input.h"
#include "Button.h"
#include "Slider.h"

class Settings : public Scene {
public:
	void update(double dt) override;
	void draw() override;
	void load() override;
	void unload() override;
private:
	Slider BGMSlider;
	Slider SFXSlider;
	Button BackButton;
};
