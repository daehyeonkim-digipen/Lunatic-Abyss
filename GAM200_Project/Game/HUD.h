#pragma once
#include "../Engine/Math.h"
#include "../Engine/GameObject.h"
#include "../Engine/Component.h"
#include <vector>

class Player;
class Texture;
class HUD : public Component {
public:
	HUD(GameObject* player, GameObject* boss);
	~HUD() override;
	void draw();
	void update(double dt) override;

private:
	static constexpr math::vec2 VALUE_OFFSET{ 2, 2 };
	bool isBossdied = false;

	struct BarInfo {
		Texture* emptyBar = nullptr;
		Texture* valueTex = nullptr;
		Texture* easingTex = nullptr;
		Texture* tipTex = nullptr;
		Texture* subTexture = nullptr; // use for charging value

		double maxValue = 0;
		double currValue = 0;

		double subValue = 0;
		double easingValue = 0;

		bool isFixed = false;
		double diff = 0;
		bool hasValueChangeChecked = false;
		bool shouldUpdateEasing = false;

		void draw(math::TransformMatrix displayMt);
		void load(std::string emptyPath, std::string valuePath, std::string easePath, std::string tipPath, double max);
		void set_sub_texture(std::string path);
		void update(double dt);

		void unload();
	};

	BarInfo playerHPBar;
	BarInfo playerLPBar;
	BarInfo bossHPBar;

	GameObject* playerPtr = nullptr; 
	GameObject* bossPtr = nullptr;

	double bossBarGoneTimer = 1;
};