#include "../Engine/Engine.h"
#include "../Engine/SceneManager.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Game.h"
#include "../Engine/Camera.h"
#include "../Engine/Font.h"
#include "../Engine/Audio.h"

#include "GOAttribute.h"
#include "Player.h"
#include "Background.h"
#include "StatScene.h"

#include <fstream>

void StatScene::load() {
	Engine::get_GE_component<Game>()->get_game_component<Background>()->add("assets/Scenes/StatSelect/tree.png", 0, 0);
	Engine::get_GE_component<Game>()->get_game_component<Background>()->add("assets/Scenes/StatSelect/fades.png", 0, 0);

	Engine::get_GE_component<Game>()->get_game_component<Camera>()->set_absolute_position({ 0, 100 });

	math::vec2 winCenterY = Engine::get_GE_component<IGL::Graphic>()->get_window_size() * math::vec2{ 0.1, 0.5 };
	double buttonsGap = 230;

	player = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_game_object<Player>(ObjType::Player);
	playerAttribute = player->get_GO_components<GOAttribute>();

	STRButton.load(winCenterY - math::vec2{ 0.0, -buttonsGap + 85.5 }, "assets/UI/Buttons/strButton.png");
	HEALTHButton.load(winCenterY - math::vec2{ 0.0, 85.5 }, "assets/UI/Buttons/hpButton.png");
	FAITHButton.load(winCenterY - math::vec2{ 0.0, buttonsGap + 85.5 }, "assets/UI/Buttons/faithButton.png");

	std::ifstream ifs("assets/Data/statInfo.txt");

	while (!ifs.eof()) {
		std::string buffer;
		std::getline(ifs, buffer);

		infoTexts.push_back(buffer);
	}

	effects = ShaderEffect(CustomShader("assets/Shaders/e_default.vert", "assets/Shaders/e_statLightRing.frag"), { 150, 150 });

	Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_IN, 0.3);

	hadSelectedStat = false;
	player->get_ps().usedFTH2inThisStage = false;
	set_info_texts();
}


void StatScene::update(double dt) {
	STRButton.update(dt);
	HEALTHButton.update(dt);
	FAITHButton.update(dt);

	player->set_position({ 0, 0 });

	SceneManager* scm = Engine::get_GE_component<Game>()->get_game_component<SceneManager>();
	Game* game = Engine::get_GE_component<Game>();

	if (scm->is_fading_done() == true && hadSelectedStat == false) {
		if (STRButton.is_button_pressed() == true) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
			playerAttribute->update_strength(1);
			if (player->get_ps().STR1 == false) {
				player->get_ps().STR1 = true;
			}
			else {
				if (player->get_ps().STR2 == false)
					player->get_ps().STR2 = true;
			}
			play_fading();
		}
		else if (HEALTHButton.is_button_pressed() == true) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
			playerAttribute->update_health(1);
			if (player->get_ps().HLT1 == false) {
				player->get_ps().HLT1 = true;
			}
			else {
				if (player->get_ps().HLT2 == false)
					player->get_ps().HLT2 = true;
			}
			play_fading();
		}
		else if (FAITHButton.is_button_pressed() == true) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
			playerAttribute->update_faith(1);
			if (player->get_ps().FTH1 == false) {
				player->get_ps().FTH1 = true;
			}
			else {
				if (player->get_ps().FTH2 == false)
					player->get_ps().FTH2 = true;
			}
			play_fading();
		}
	}
	else if (scm->is_fading_done() == true && hadSelectedStat == true) {
		playerAttribute->set_ingame_status();
		scm->set_next_scene(game->statNextScene);
		scm->play_fading(SceneManager::MODE::FADE_IN, 1.2);
	}

}

void StatScene::draw() {
	Engine::get_GE_component<IGL::Graphic>()->set_background_color(0, 0, 0, 255);
	auto cam = Engine::get_GE_component<Game>()->get_game_component<Camera>();
	Engine::get_GE_component<Game>()->get_game_component<Background>()->draw(cam->get_matrix());

	STRButton.draw();
	HEALTHButton.draw();
	FAITHButton.draw();

	glm::vec4 color{ 0 };
	if (STRButton.is_mouse_on_button()) {
		color = { 1, 0, 0, 1 };
	}
	else if (HEALTHButton.is_mouse_on_button()) {
		color = { 0, 1, 0, 1 };
	}
	else if (FAITHButton.is_mouse_on_button()) {
		color = { 0, 0, 1, 1 };
	}

	effects.set_variable(Uniforms::Color, color);

	effects.draw(cam->get_matrix() * math::TranslateMatrix{ { 500, 150 } });
	player->draw(cam->get_matrix() * math::TranslateMatrix{ { 575, 165 } } *math::ScaleMatrix{ { -1, 1 } });



	auto pps = player->get_ps();
	Font* font = Engine::get_GE_component<Game>()->get_game_component<Font>();

	math::vec2 infoPosOff{ 165, 80 }; // center of button
	math::vec2 statOff = infoPosOff + math::vec2{ 0, 40 };
	math::ScaleMatrix s{ { 1.2, 1.2 } };
	font->draw_text_to_texture(math::TranslateMatrix{ STRButton.get_position() + statOff } *s, infoTexts[I_STR_STAT]);
	font->draw_text_to_texture(math::TranslateMatrix{ HEALTHButton.get_position() + statOff } *s, infoTexts[I_HLT_STAT]);
	font->draw_text_to_texture(math::TranslateMatrix{ FAITHButton.get_position() + statOff } *s, infoTexts[I_FTH_STAT]);

	//	1		2	
	//	true	false 	-> 2 info show
	//	true	true	-> nothing show
	//	false	false	-> 1 info show
	//	false	true	x

	math::vec2 skillOff = infoPosOff + math::vec2{ 0, -40 };

	font->draw_text_to_texture(math::TranslateMatrix{ STRButton.get_position() + skillOff } *s, strSkillInfo);
	font->draw_text_to_texture(math::TranslateMatrix{ HEALTHButton.get_position() + skillOff } *s, hltSkillInfo);
	font->draw_text_to_texture(math::TranslateMatrix{ FAITHButton.get_position() + skillOff } *s, fthSkillInfo);

}

void StatScene::unload() {
	Engine::get_GE_component<Game>()->get_game_component<Background>()->clear();

	STRButton.unload();
	HEALTHButton.unload();
	FAITHButton.unload();

	effects.remove_shader();
}

void StatScene::play_fading() {
	hadSelectedStat = true;
	Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_OUT, 1.2);
}

void StatScene::set_info_texts() {
	auto pps = player->get_ps();

	if (pps.STR2 == false) {
		strSkillInfo = pps.STR1 ? infoTexts[I_STR_SKILL2] : infoTexts[I_STR_SKILL1];
	}
	else strSkillInfo.clear();

	if (pps.HLT2 == false) {
		hltSkillInfo = pps.HLT1 ? infoTexts[I_HLT_SKILL2] : infoTexts[I_HLT_SKILL1];
	}
	else hltSkillInfo.clear();

	if (pps.FTH2 == false) {
		fthSkillInfo = pps.FTH1 ? infoTexts[I_FTH_SKILL2] : infoTexts[I_FTH_SKILL1];
	}
	else fthSkillInfo.clear();
}
