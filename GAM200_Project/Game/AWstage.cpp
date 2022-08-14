#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Graphic.h"
#include "../Engine/Camera.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Audio.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/TextureManager.h"

#include "GOAttribute.h"
#include "HUD.h"
#include "GameMap.h"
#include "Player.h"
#include "Gravity.h"
#include "Cinematic.h"
#include "CollisionBlock.h"
#include "Background.h"
#include "Frontground.h"
#include "AncientWizard.h"
#include "AWStage.h"

void AWStage::load() {
	Game* game = Engine::get_GE_component<Game>();
	auto GOM = game->get_game_component<GameObjectManager>();
	game->get_game_component<Camera>()->set_absolute_position({ 61, 0 });

	gameSceneComponentManager.add_component(new Gravity(15));
	gameSceneComponentManager.add_component(new GameMap("assets/Scenes/Ancient/", "terrain.png", "map_layer.png"));
	gameSceneComponentManager.add_component(new Cinematic());
	Engine::get_GE_component<Audio>()->play_bgm(&Engine::get_GE_component<Audio>()->BGMForest);

	//game->get_game_component<Frontground>()->add("assets/Scenes/DarkBoar/grass.png", 0, 0);
	game->get_game_component<Camera>()->set_cam_extent({ { 64, 0 }, get_GS_components<GameMap>()->get_stage_size() });
	game->get_game_component<Camera>()->set_absolute_position({ game->get_game_component<Camera>()->get_position().x, 0. });

	blockPos.push_back({ 1099, 1000 - 739 });
	blockPos.push_back({ 1099, 1000 - 304 });
	blockPos.push_back({ 594, 1000 - 351 });
	blockPos.push_back({ 594, 1000 - 687 });
	blockPos.push_back({ 185, 1000 - 395 });
	blockPos.push_back({ 185, 1000 - 644 });
	blockPos.push_back({ 1602, 1000 - 351 });
	blockPos.push_back({ 1602, 1000 - 687 });
	blockPos.push_back({ 2007, 1000 - 395 });
	blockPos.push_back({ 2007, 1000 - 644 });

	Player* player = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_game_object<Player>(ObjType::Player);
	player->set_position({ blockPos[0] });
	AncientWizard* boss = new AncientWizard({ blockPos[1] }, player);
	GOM->add_game_object(ObjType::Enemy, boss);
	gameSceneComponentManager.add_component(new HUD(player, boss));

	bossDeadTimer = 0;
	hasPlayedFading = false;

}

void AWStage::unload() {
	gameSceneComponentManager.clear_components();
	Engine::get_GE_component<Game>()->get_game_component<Background>()->clear();
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->clear_game_object();
	Engine::get_GE_component<Game>()->get_game_component<Frontground>()->clear();

}

void AWStage::update(double dt) {
	Game* game = Engine::get_GE_component<Game>();
	Camera* cam = game->get_game_component<Camera>();
	GameObjectManager* GOM = game->get_game_component<GameObjectManager>();
	Player* player = GOM->get_game_object<Player>(ObjType::Player);
	AncientWizard* boss = GOM->get_game_object<AncientWizard>(ObjType::Enemy);

	//FallenAngel* Boss = GOM->get_game_object<FallenAngel>(ObjType::Enemy);
	//math::vec2 viewPortSize = Engine::get_GE_component<IGL::Graphic>()->get_viewport_size();

	gameSceneComponentManager.update_all(dt);

	//math::vec2 viewPortCenter = cam->get_position() + math::vec2{ viewPortSize.x, 0.0 };
	//Engine::get_GE_component<AtmoSphericalDustEmitter>()->update_emit(dt, 30, viewPortCenter, viewPortSize * 1.5);
	// Engine::get_GE_component<StompParticleEmitter>()->emit(10, { 300, 300 }, { 10, 3 }, {10, 1}, 90);
	if(boss && boss->is_teleported())
	{
		math::vec2 pos;
		if(boss->is_ult())
		{
			pos = blockPos[0];
		} else
		{
			pos = blockPos[rand() % 10];
		}
		
		boss->set_position(pos);
		boss->set_teleported();
	}


	if (player == nullptr) return;

	if (boss == nullptr) {
		if (hasPlayedFading == false) {
			game->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_OUT, 7);
			hasPlayedFading = true;
		}

		bossDeadTimer += dt;
		if (bossDeadTimer >= 7) {
			game->get_game_component<SceneManager>()->set_next_scene(Scenes::Mainmenu);
		}
	}

	if (!Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_collidable())
	{
		cam->cam_update_for(player);
	}

	if(player->get_position().y < -500)
	{
		player->set_position({ player->get_position().x, get_GS_components<GameMap>()->get_stage_size().y + 500 });
	}

	if (player->get_position().x < -100)
	{
		player->set_position({ get_GS_components<GameMap>()->get_stage_size().x + 100, player->get_position().y });
	} else if(player->get_position().x > get_GS_components<GameMap>()->get_stage_size().x + 100)
	{
		player->set_position({ -100., player->get_position().y });

	}
}

void AWStage::draw() {
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
}