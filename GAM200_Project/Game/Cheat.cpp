#include "../Engine/Game.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"

#include "Player.h"
#include "GOAttribute.h"
#include "Cheat.h"
#include <iostream> // std::cin

Cheat::Cheat(GameKey key)
	: CheatKey(key)
{}

void Cheat::update(double) {
	if (CheatKey.is_key_pressed()) {
		std::string cheatCode;

		std::cin.clear();
		while (std::cin >> cheatCode) {} 
		std::cin.clear();
		
		if (cheatCode == "tuto") {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::Tutorial);
		}
		else if (cheatCode == "stat") {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::StatStage);
		}
		else if (cheatCode == "boar") {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::DarkBoarStage);
		}
		else if (cheatCode == "tree") {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::SkullTreeStage);
		}
		else if (cheatCode == "fallen") {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::FallenAngelStage);
		}
		else if (cheatCode == "ancient") {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::AncientWizardStage);
		}
		else if (cheatCode == "invincible") {
			Player* p = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_game_object<Player>(ObjType::Player);
			p->get_GO_components<GOAttribute>()->set_max_hp(100000);
			p->get_GO_components<GOAttribute>()->update_hp(100000 - p->get_GO_components<GOAttribute>()->get_hp());
		}
		else if (cheatCode == "suicide") {
			Player* p = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->get_game_object<Player>(ObjType::Player);
			p->set_is_dead(true);
		}
		else if (cheatCode == "fade") {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_IN, 5);
		}
		else {
			Engine::get_logger().log_debug("unknown command : " + cheatCode);
		}
	}
}
