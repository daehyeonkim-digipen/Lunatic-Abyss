#include "Camera.h"
#include "Engine.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Input.h"
#include "Game.h"

#include "Font.h"
#include "ShowCollision.h"

#include "../Game/Background.h"
#include "../Game/Frontground.h"
#include "../Game/Particles.h"
#include "../Game/Cheat.h"


#include "../Game/Player.h" // finding player

Game::Game()  {
	math::vec2 viewportSize = Engine::get_GE_component<IGL::Graphic>()->get_viewport_size();
	gameComponentManager.add_component(new Camera());
	gameComponentManager.get_component<Camera>()->set_movable_range({ { viewportSize.x * 0.49, viewportSize.y * 0.505 }, viewportSize * 0.51 });

	gameComponentManager.add_component(new SceneManager());
	gameComponentManager.add_component(new ShowCollision(InputKey::Keyboard::Tilde));
	gameComponentManager.add_component(new Background());
	gameComponentManager.add_component(new Frontground());
	gameComponentManager.add_component(new Font("assets/UI/Font.png"));

	gameComponentManager.add_component(new GameObjectManager());

#ifdef _DEBUG
	gameComponentManager.add_component(new Cheat(InputKey::Keyboard::Down));
#endif // DEBUG

	hasPlayerAdded = false;
	hasPlayedFading = false;

}

Game::~Game() {
	gameComponentManager.clear_components();
}

void Game::set_hitstop() {
	hitstopTimer = 0.25;
}

void Game::update(double dt) {
	SceneManager* scm = gameComponentManager.get_component<SceneManager>();
	auto key_pressed = [=](GameKey k) ->bool { return Engine::get_GE_component<Input>()->is_key_pressed(k); };

	if (key_pressed(GameKey::Escape)) {
		if (!scm->is_current_scene(Scenes::Mainmenu)) {
			scm->set_next_scene(Scenes::Mainmenu);
			scm->play_fading(SceneManager::MODE::FADE_IN, 2.5);
			hasPlayerAdded = false;
		}
		else
			scm->set_next_scene(Scenes::Exit);
	}

	if (hitstopTimer > 0) {
		hitstopTimer -= dt;
		if (hitstopTimer <= 0) {
			hitstopTimer = -1;
		}
	}


	gameComponentManager.update_all(dt);
	gameComponentManager.get_component<GameObjectManager>()->collide_each_object();
	if(hitstopTimer <= 0) gameComponentManager.get_component<GameObjectManager>()->update_entire_position();

	Player* player = gameComponentManager.get_component<GameObjectManager>()->get_game_object<Player>(ObjType::Player);
	if (hasPlayerAdded == true && player == nullptr) { // player died
		if (hasPlayedFading == false) {
			scm->play_fading(SceneManager::MODE::FADE_OUT, 4);
			hasPlayedFading = true;
		}

		if (scm->is_fading_done() == true) {
			scm->set_next_scene(Scenes::Mainmenu);
			scm->play_fading(SceneManager::MODE::FADE_IN, 2.5);
			hasPlayerAdded = false;
			hasPlayedFading = false;
		}
	}

	scm->draw_fade_effect();
}
