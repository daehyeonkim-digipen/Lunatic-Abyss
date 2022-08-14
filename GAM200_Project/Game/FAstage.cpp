#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Graphic.h"
#include "../Engine/Camera.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Audio.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Sprite.h"

#include "GOAttribute.h"
#include "HUD.h"
#include "GameMap.h"
#include "Player.h"
#include "Gravity.h"
#include "Cinematic.h"
#include "Background.h"
#include "Frontground.h"
#include "FallenAngel.h"
#include "FAStage.h"


void FAStage::load() {
	Game* game = Engine::get_GE_component<Game>();
	auto GOM = game->get_game_component<GameObjectManager>();
	game->get_game_component<Camera>()->set_absolute_position({ 61, 0 });

	gameSceneComponentManager.add_component(new Gravity(15));
	gameSceneComponentManager.add_component(new GameMap("assets/Scenes/Fallen/", "terrain.png", "map_layer.png"));
	gameSceneComponentManager.add_component(new Cinematic());
	Engine::get_GE_component<Audio>()->play_bgm(&Engine::get_GE_component<Audio>()->BGMForest);

	//game->get_game_component<Frontground>()->add("assets/Scenes/DarkBoar/grass.png", 0, 0);
	game->get_game_component<Camera>()->set_cam_extent({ { 1100, 0 }, {2200., get_GS_components<GameMap>()->get_stage_size().y} });
	game->get_game_component<Camera>()->set_absolute_position({game->get_game_component<Camera>()->get_extent().point1.x, 3800.});
	Player* player = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_game_object<Player>(ObjType::Player);
	FallenAngel* boss = new FallenAngel({ get_GS_components<GameMap>()->get_stage_size().x * 0.5, 3930. }, player);
	
	GOM->add_game_object(ObjType::Enemy, boss);
	player->set_position({ 1220, 4000 });
	gameSceneComponentManager.add_component(new HUD(player, boss));
	light = new GameObject({ 0, 0 });
	light->add_GO_component(new Sprite("assets/Sprite/Particles/Moonlight.spt", light));
	bossDeadTimer = 0;
	hasPlayerFading = false;
}

void FAStage::unload() {
	gameSceneComponentManager.clear_components();
	Engine::get_GE_component<Game>()->get_game_component<Background>()->clear();
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->clear_game_object();
	Engine::get_GE_component<Game>()->get_game_component<Frontground>()->clear();

}

void FAStage::update(double dt) {
	Game* game = Engine::get_GE_component<Game>();
	Camera* cam = game->get_game_component<Camera>();
	GameObjectManager* GOM = game->get_game_component<GameObjectManager>();
	Player* player = GOM->get_game_object<Player>(ObjType::Player);
	FallenAngel* Boss = GOM->get_game_object<FallenAngel>(ObjType::Enemy);
	math::vec2 viewPortSize = Engine::get_GE_component<IGL::Graphic>()->get_viewport_size();

	gameSceneComponentManager.update_all(dt);

	//math::vec2 viewPortCenter = cam->get_position() + math::vec2{ viewPortSize.x, 0.0 };
	//Engine::get_GE_component<AtmoSphericalDustEmitter>()->update_emit(dt, 30, viewPortCenter, viewPortSize * 1.5);
	// Engine::get_GE_component<StompParticleEmitter>()->emit(10, { 300, 300 }, { 10, 3 }, {10, 1}, 90);

	if (player == nullptr) return;

	if (hasInitHUD == false && Boss->get_phase() == FallenAngel::Phase::phase2) {
		hasInitHUD = true;
		gameSceneComponentManager.delete_component<HUD>();
		gameSceneComponentManager.add_component(new HUD(player, Boss));
	}

	if(!Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_collidable())
	{
		cam->cam_update_for(player);
	}

	if (Boss == nullptr) {
		if (hasPlayerFading == false) {
			game->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_OUT, 5);
			hasPlayerFading = true;
		}

		bossDeadTimer += dt;
		if (bossDeadTimer >= 5) {
			game->statNextScene = Scenes::AncientWizardStage;
			game->get_game_component<SceneManager>()->set_next_scene(Scenes::StatStage);
		}
	}

	if (player->get_scale().x < 0 && player->get_position().x > cam->get_extent().Right() + Engine::get_GE_component<IGL::Graphic>()->get_viewport_size().x - 100 && player->is_player_bloking())
	{
		gimmickTimer -= dt;
		auto BAttri = Boss->get_GO_components<GOAttribute>();
		if(BAttri && gimmickTimer < 0 && Boss->get_phase() == FallenAngel::Phase::phase1)
		{
			lighton = true;
			double ptob = Boss->get_position().x - player->get_position().x;
			double Xscale = ptob / 146.;
			light->set_scale({ -Xscale, 1. });
			light->set_position(player->get_position() + math::vec2{-10, 20});
			BAttri->update_hp(-1);
		}
	} else
	{
		lighton = false;
		gimmickTimer = 1.2;
	}
}

void FAStage::draw() {
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

	if(lighton)
	{
		light->draw(camPtr->get_matrix());
	}

	cinema->draw();
}