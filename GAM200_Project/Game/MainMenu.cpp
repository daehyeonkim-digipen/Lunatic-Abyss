#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/Game.h"
#include "../Engine/SceneManager.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/Audio.h"
#include "../Engine/Scene.h"
#include "MainMenu.h"
#include "Settings.h"
#include "Background.h"
#include "Player.h"

void MainMenu::load()
{
	Game* game = Engine::get_GE_component<Game>();

	for (int i = 0; i < 3; i++)
		buttons.push_back(Button());

	buttons[0].load(math::vec2(200, 300), "assets/UI/Buttons/start.png");
	buttons[1].load(math::vec2(190, 220), "assets/UI/Buttons/setting.png");
	buttons[2].load(math::vec2(200, 140), "assets/UI/Buttons/quit.png");

	game->get_game_component<Background>()->add("assets/Scenes/Main/main.png", 0, 0);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->delete_game_object<Player>(ObjType::Player);

	Engine::get_GE_component<Audio>()->play_bgm(&Engine::get_GE_component<Audio>()->BGMMainMenu);
	game->get_game_component<GameObjectManager>()->set_lock();

	hasPlayedFading = false;
	Engine::get_GE_component<Input>()->set_key_work(true);
	selectedIndex = -1;
}

void MainMenu::update(double dt)
{
	Engine::get_GE_component<Game>()->get_game_component<Camera>()->set_absolute_position({ 0, 0 });

	SceneManager* scm = Engine::get_GE_component<Game>()->get_game_component<SceneManager>();

	if (scm->is_fading_done() == true)
		for (Button& button : buttons)
			button.update(dt);

	if (scm->is_fading_done() == true && selectedIndex == -1) {
		if (buttons[0].is_button_pressed()) {
			selectedIndex = 0;
			scm->play_fading(SceneManager::MODE::FADE_OUT, 2);
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
		}
		else if (buttons[1].is_button_pressed()) {
			selectedIndex = 1;
			scm->play_fading(SceneManager::MODE::FADE_OUT, 0.2);
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
		}
		else if (buttons[2].is_button_pressed()) {
			selectedIndex = 2;
			scm->play_fading(SceneManager::MODE::FADE_OUT, 1);
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
		}
	}

	if (selectedIndex > -1 && scm->is_fading_done() == true) { // has selected and fading effect also has done
		if (selectedIndex == 0) {
			scm->set_next_scene(Scenes::Tutorial);
			scm->play_fading(SceneManager::MODE::FADE_IN, 1);
		}
		else if (selectedIndex == 1) {
			scm->set_next_scene(Scenes::Setting);
			scm->play_fading(SceneManager::MODE::FADE_IN, 0.2);
		}
		else if (selectedIndex == 2) {
			scm->set_next_scene(Scenes::Exit);
		}
	}
}

void MainMenu::draw() {
	Engine::get_GE_component<IGL::Graphic>()->set_background_color(120, 120, 120, 255);
	auto cam = Engine::get_GE_component<Game>()->get_game_component<Camera>();
	
	Engine::get_GE_component<Game>()->get_game_component<Background>()->draw(cam->get_matrix());

	for (Button& button : buttons)
		button.draw();

}

void MainMenu::unload() {
	Engine::get_GE_component<Game>()->get_game_component<Background>()->clear();
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->unlock();


	for (Button& button : buttons)
		button.unload();

	buttons.clear();
}
