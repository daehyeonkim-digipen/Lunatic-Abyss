#include "../Engine/TextureManager.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Engine.h"
#include "../Engine/Texture.h"
#include "../Engine/Graphic.h"
#include "../Engine/Game.h"
#include "../Engine/Font.h"

#include "GOAttribute.h"
#include "Player.h"
#include "HUD.h"

#include <typeinfo>

HUD::HUD(GameObject* player_, GameObject* boss_)
	: playerPtr(player_), bossPtr(boss_)
{
	GOAttribute* p_attrib = playerPtr->get_GO_components<GOAttribute>();
	GOAttribute* b_attrib = bossPtr->get_GO_components<GOAttribute>();

	playerHPBar.load("assets/HUD/Player/HP_empty.png", "assets/HUD/Player/HP.png", "assets/HUD/Player/HP_easing.png", "assets/HUD/Player/HP_tip.png", p_attrib->get_max_hp());
	playerLPBar.load("assets/HUD/Player/LP_empty.png", "assets/HUD/Player/LP.png", "assets/HUD/Player/LP_easing.png", "assets/HUD/Player/LP_tip.png", p_attrib->get_max_gage());
	playerLPBar.set_sub_texture("assets/HUD/Player/LP_charge.png");
	bossHPBar.load("assets/HUD/Boss/HP_empty.png", "assets/HUD/Boss/HP.png", "assets/HUD/Boss/HP_easing.png", "", b_attrib->get_max_hp());
	bossHPBar.isFixed = true;
}

HUD::~HUD() {
	playerHPBar.unload();
	playerLPBar.unload();
	bossHPBar.unload();
}

void HUD::update(double dt) {
	if (!isBossdied && bossPtr->get_GO_components<GOAttribute>() == nullptr) {
		isBossdied = true;
	}
	if (isBossdied == true && bossBarGoneTimer > 0)
		bossBarGoneTimer -= dt;

	if(!isBossdied)
	{
		GOAttribute* p_attrib = playerPtr->get_GO_components<GOAttribute>();
		GOAttribute* b_attrib = bossPtr->get_GO_components<GOAttribute>();

		if (p_attrib != nullptr) {
			playerHPBar.currValue = p_attrib->get_hp();
			playerLPBar.currValue = p_attrib->get_gage();
			playerLPBar.subValue = p_attrib->get_charge();

			playerHPBar.update(dt);
			playerLPBar.update(dt);
		}

		if (b_attrib != nullptr) {
			bossHPBar.currValue = b_attrib->get_hp();
			bossHPBar.update(dt);

			bossHPBar.shouldUpdateEasing = b_attrib->get_taken_damage() == 0;
		}
	}
}

void HUD::draw() {
	math::vec2 winSize = Engine::get_GE_component<IGL::Graphic>()->get_window_size();
	math::ScaleMatrix scaleOff{ math::vec2{ 2, 2 } };

	// =============== boss ===============
	math::vec2 bossBarOffset = winSize * math::vec2{ 0.2, 0.05 };
	math::TransformMatrix bossOffsetMatrix = math::TranslateMatrix{ bossBarOffset } *scaleOff;

	if (bossBarGoneTimer > 0) {
		bossHPBar.draw(bossOffsetMatrix);

		math::TransformMatrix bossNameOffsetMatrix = math::TranslateMatrix{ bossBarOffset + math::vec2{ 10, 30 } };
		math::TransformMatrix bossTakenDamageMt = math::TranslateMatrix{ bossBarOffset + math::vec2{ 895, 30 } };
		Font* font = Engine::get_GE_component<Game>()->get_game_component<Font>();

		font->draw_text_to_texture(bossNameOffsetMatrix, bossPtr->get_name());
		GOAttribute* batt = bossPtr->get_GO_components<GOAttribute>();
		if (batt) {
			double viewDamage = batt->get_taken_damage();
			if (viewDamage != 0)
				font->draw_text_to_texture(bossTakenDamageMt, std::to_string((int)viewDamage));
		}
	}
	


	// =============== player ===============
	math::vec2 PHPBarOffset = winSize * math::vec2{ 0.035, 0.9 };
	math::TransformMatrix PHPOffsetMatrix = math::TranslateMatrix{ PHPBarOffset } *scaleOff;
	math::vec2 PLPBarOffset = PHPBarOffset - math::vec2{ 0, 20 };
	math::TransformMatrix PLPOffsetMatrix = math::TranslateMatrix{ PLPBarOffset } *scaleOff;

	playerHPBar.draw(PHPOffsetMatrix);
	playerLPBar.draw(PLPOffsetMatrix);
	

}

// ================== barInfo =================================

void HUD::BarInfo::load(std::string emptyPath, std::string valuePath, std::string easePath, std::string tipPath, double max) {
	TextureManager* txm = Engine::get_GE_component<TextureManager>();

	if (emptyPath.empty() == false)
		emptyBar = txm->load(emptyPath);
	if (valuePath.empty() == false)
		valueTex = txm->load(valuePath);
	if (easePath.empty() == false)
		easingTex = txm->load(easePath);
	if (tipPath.empty() == false)
		tipTex = txm->load(tipPath);

	maxValue = max;
	currValue = max;
	easingValue = 0;
}

void HUD::BarInfo::set_sub_texture(std::string path) {
	subTexture = Engine::get_GE_component<TextureManager>()->load(path);
}

void HUD::BarInfo::update(double dt) {	
	if (easingValue != currValue) {
		if (hasValueChangeChecked == false) {
			diff = abs(currValue - easingValue);
			hasValueChangeChecked = true;
		}

		if (easingValue < currValue) {
			easingValue += diff * dt;
			if (abs(easingValue - currValue) < 0.1) {
				easingValue = currValue;
			}
		}
		else {
			easingValue -= diff * dt;
			if (abs(easingValue - currValue) < 0.1) {
				easingValue = currValue;
			}
		}
	}

	if (shouldUpdateEasing == true) { // attrib -> taken damage is 0 -> 1 sec passed after get damaged
		hasValueChangeChecked = false;
	}
}

void HUD::BarInfo::draw(math::TransformMatrix displayMt) {
	if (emptyBar != nullptr)
		emptyBar->draw(displayMt, { 0, 0 }, { isFixed ? emptyBar->get_size().x : maxValue * 1.7, emptyBar->get_size().y });

	if (easingTex != nullptr) {
		double FixedValue = (easingValue / maxValue) * easingTex->get_size().x;
		easingTex->draw(displayMt * math::TranslateMatrix{ VALUE_OFFSET }, { 0, 0 }, { isFixed ? FixedValue : easingValue * 1.7, easingTex->get_size().y });
	}

	if (valueTex != nullptr) {
		double FixedValue = (currValue / maxValue) * valueTex->get_size().x;
		valueTex->draw(displayMt * math::TranslateMatrix{ VALUE_OFFSET }, { 0, 0 }, { isFixed ? FixedValue : currValue * 1.7, valueTex->get_size().y });
	}

	if (subTexture != nullptr)
		subTexture->draw(displayMt * math::TranslateMatrix{ VALUE_OFFSET }, { 0, 0 },  { subValue * 1.7, subTexture->get_size().y });

	if (tipTex != nullptr) 
		tipTex->draw(displayMt * math::TranslateMatrix{ math::vec2{ maxValue * 1.7, 0.0 } });
}

void HUD::BarInfo::unload() {
	TextureManager* txm = Engine::get_GE_component<TextureManager>();

	if (emptyBar != nullptr)
		txm->remove_texture(emptyBar);
	if (valueTex != nullptr)
		txm->remove_texture(valueTex);
	if (easingTex != nullptr)
		txm->remove_texture(easingTex);
	if (tipTex != nullptr)
		txm->remove_texture(tipTex);
	if (subTexture != nullptr)
		txm->remove_texture(subTexture);
}