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
#include "Background.h"
#include "Frontground.h"
#include "CollisionBlock.h"
#include "DarkBoar.h"

#include "DBstage.h"

void DarkBoarStage::load() {
	Game* game = Engine::get_GE_component<Game>();
	auto GOM = game->get_game_component<GameObjectManager>();
	game->get_game_component<Camera>()->set_absolute_position({ 61, 0 });

	gameSceneComponentManager.add_component(new Gravity(15));
	gameSceneComponentManager.add_component(new GameMap("assets/Scenes/DarkBoar/", "terrain.png", "map_layer.png"));
	gameSceneComponentManager.add_component(new Cinematic());
	Engine::get_GE_component<Audio>()->play_bgm(&Engine::get_GE_component<Audio>()->BGMForest);

	game->get_game_component<Background>()->add("assets/Scenes/DarkBoar/back_1st.png", 0, 0);
	game->get_game_component<Frontground>()->add("assets/Scenes/DarkBoar/grass.png", 0, 0);
	game->get_game_component<Camera>()->set_cam_extent({ { 64, -1000 }, get_GS_components<GameMap>()->get_stage_size() });

	Player* player = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_game_object<Player>(ObjType::Player);

	DarkBoar* boss = new DarkBoar({ get_GS_components<GameMap>()->get_stage_size().x / 2 + 50, 200.0 }, player);
	clearFlagBlock = new CollisionBlock({ { get_GS_components<GameMap>()->get_stage_size().x, 169.0 }, { 100, 500 } });

	GOM->add_game_object(ObjType::Enemy, boss);
	GOM->add_game_object(ObjType::Block, clearFlagBlock);
	player->set_position({ 170, 200 });
	gameSceneComponentManager.add_component(new HUD(player, boss));

	bossDeadTimer = 0;
	hasPlayedFading = false;
}

void DarkBoarStage::unload() {
	gameSceneComponentManager.clear_components();
	Engine::get_GE_component<Game>()->get_game_component<Background>()->clear();
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->clear_game_object();
	Engine::get_GE_component<Game>()->get_game_component<Frontground>()->clear();

}

void DarkBoarStage::update(double dt) {
	Game* game = Engine::get_GE_component<Game>();
	Camera* cam = game->get_game_component<Camera>();
	GameObjectManager* GOM = game->get_game_component<GameObjectManager>();
	Player* player = GOM->get_game_object<Player>(ObjType::Player);

	gameSceneComponentManager.update_all(dt);
	
	if (player == nullptr) return;

	cam->cam_update_for(player);


	if (GOM->get_game_object<DarkBoar>(ObjType::Enemy) == nullptr) {
		if (hasPlayedFading == false) {
			game->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_OUT, 5);
			hasPlayedFading = true;
		}

		bossDeadTimer += dt;
		if (bossDeadTimer >= 5) {
			game->statNextScene = Scenes::SkullTreeStage;
			game->get_game_component<SceneManager>()->set_next_scene(Scenes::StatStage);
		}
	}
}

void DarkBoarStage::draw() {
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
