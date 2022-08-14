#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Graphic.h"
#include "../Engine/Camera.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Audio.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Font.h"

#include "GOAttribute.h"
#include "HUD.h"
#include "GameMap.h"
#include "Player.h"
#include "Gravity.h"
#include "Cinematic.h"
#include "Background.h"
#include "Frontground.h"
#include "Trainer.h"
#include "TutorialStage.h"

#include "../Engine/ShaderEffect.h"

TutorialStage::TutorialStage() 
	: hintReviewKey(InputKey::Keyboard::H), hintSkipKey(InputKey::Keyboard::Right),
		elapsedTime(0), hintIndex(0)
{}

void TutorialStage::load() {
	Game* game = Engine::get_GE_component<Game>();
	auto GOM = game->get_game_component<GameObjectManager>();
	game->get_game_component<Camera>()->set_absolute_position({ 64, 0 });

	gameSceneComponentManager.add_component(new Gravity(15));
	gameSceneComponentManager.add_component(new GameMap("assets/Scenes/Tutorial/", "blocks.png", "floor.png"));
	gameSceneComponentManager.add_component(new Cinematic());
	Engine::get_GE_component<Audio>()->play_bgm(&Engine::get_GE_component<Audio>()->BGMForest);

	game->get_game_component<Background>()->add("assets/Scenes/Tutorial/parallax_1st.png", 0, 0);
	game->get_game_component<Camera>()->set_cam_extent({ { 64, -1000 }, get_GS_components<GameMap>()->get_stage_size() });

	Player* player = new Player({ 170, 200 });
	game->send_player_added();
	GOM->add_game_object(ObjType::Player, player);

	Trainer* trainer = new Trainer({ get_GS_components<GameMap>()->get_stage_size().x * 0.75, 200.0 }, player);
	GOM->add_game_object(ObjType::Enemy, trainer);

	gameSceneComponentManager.add_component(new HUD(player, trainer));
	elapsedTime = 0;
	hintIndex = 0;

	std::ifstream hintInput("assets/Data/TutorialHints.txt");

	std::string bufferStr;
	while (!hintInput.eof()) {
		std::getline(hintInput, bufferStr);
		hints.push_back(bufferStr);
		bufferStr.clear();
	}

	hintInput.close();

	hint_cinematic_play();

	reviewInfo = "Press [H] to review the instruction.";
}

void TutorialStage::unload() {
	gameSceneComponentManager.clear_components();
	Engine::get_GE_component<Game>()->get_game_component<Background>()->clear();
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->clear_game_object();
	Engine::get_GE_component<Game>()->get_game_component<Frontground>()->clear();
}

void TutorialStage::update(double dt) {
	Game* game = Engine::get_GE_component<Game>();
	Camera* cam = game->get_game_component<Camera>();
	GameObjectManager* GOM = game->get_game_component<GameObjectManager>();
	Player* player = GOM->get_game_object<Player>(ObjType::Player);

	gameSceneComponentManager.update_all(dt);

	if (player == nullptr) return;

	cam->cam_update_for(player);

	if (elapsedTime >= 0) {
		elapsedTime += dt;

		if (hintSkipKey.is_key_pressed() == true) {
			elapsedTime = 6; 
		}

		if (elapsedTime > 5) {
			elapsedTime = 0;

			if (hintIndex < hints.size() - 1)
				hintIndex++;
			else {
				elapsedTime = -1;
				gameSceneComponentManager.get_component<Cinematic>()->stop();
			}
		}
	}
	
	if (elapsedTime == -1 && hintReviewKey.is_key_pressed() == true) {
		elapsedTime = 0;
		hintIndex = 0;
		hint_cinematic_play();
	}

	if (player->get_position().x > cam->get_extent().Right() + Engine::get_GE_component<IGL::Graphic>()->get_viewport_size().x + 100) {
		Engine::get_GE_component<Game>()->statNextScene = Scenes::DarkBoarStage;
		Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::StatStage);
	}
}

void TutorialStage::draw() {
	Engine::get_GE_component<IGL::Graphic>()->set_background_color(10, 10, 20, 255);
	Camera* camPtr = Engine::get_GE_component<Game>()->get_game_component<Camera>();

	Engine::get_GE_component<Game>()->get_game_component<Background>()->draw(camPtr->get_matrix());

	gameSceneComponentManager.get_component<GameMap>()->draw();

	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->draw_all_game_object(camPtr->get_matrix());

	Engine::get_GE_component<GameObjectManager>()->draw_all_game_object(camPtr->get_matrix());

	Engine::get_GE_component<Game>()->get_game_component<Frontground>()->draw(camPtr->get_matrix());

	auto cinema = gameSceneComponentManager.get_component<Cinematic>();
	if (cinema->is_playing() == false) {
		gameSceneComponentManager.get_component<HUD>()->draw();
	}

	cinema->draw();

	Font* font = Engine::get_GE_component<Game>()->get_game_component<Font>();
	double scaleAmt = 1.5;
	math::vec2 fontXCenterOffset = { (elapsedTime != -1 ? font->measure_text(hints[hintIndex]).x / 2 : font->measure_text(reviewInfo).x / 2) * scaleAmt, 0.0 };
	math::vec2 winOffset = Engine::get_GE_component<IGL::Graphic>()->get_window_size() * math::vec2{ 0.5, 0.91 };

	font->draw_text_to_texture(math::TranslateMatrix{ winOffset - fontXCenterOffset } *math::ScaleMatrix{ { scaleAmt, scaleAmt } }, elapsedTime != -1 ? hints[hintIndex] : reviewInfo);

}

void TutorialStage::hint_cinematic_play() {
	double time = 5 * ((double)hints.size() - 1 - hintIndex);
	gameSceneComponentManager.get_component<Cinematic>()->play(time);
}
