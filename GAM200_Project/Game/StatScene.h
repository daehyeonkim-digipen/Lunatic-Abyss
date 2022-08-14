#pragma once
#include "../Engine/Scene.h"
#include "../Engine/ShaderEffect.h"
#include "Button.h"

class GOAttribute;
class Player;
class StatScene : public Scene {
public:

	void update(double dt) override;
	void draw() override;
	void load() override;
	void unload() override;
private:
	void play_fading();
	void set_info_texts();

	enum InfoType { 
		I_STR_STAT, I_STR_SKILL1, I_STR_SKILL2,
		I_HLT_STAT, I_HLT_SKILL1, I_HLT_SKILL2,
		I_FTH_STAT, I_FTH_SKILL1, I_FTH_SKILL2,
		I_count
	};

	std::vector<std::string> infoTexts;

	Button STRButton;
	Button HEALTHButton;
	Button FAITHButton;

	std::string strSkillInfo;
	std::string hltSkillInfo;
	std::string fthSkillInfo;

	GOAttribute* playerAttribute;
	Player* player;

	ShaderEffect effects;

	bool hadSelectedStat = false;
};

