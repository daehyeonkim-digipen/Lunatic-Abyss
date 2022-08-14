#include "Settings.h"
#include "../Engine/Scene.h"
#include "../Engine/Engine.h"
#include "../Engine/SceneManager.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Game.h"
#include "../Engine/Audio.h"
#include "../Engine/Camera.h"
#include "Background.h"

void Settings::load() {
	Engine::get_GE_component<Game>()->get_game_component<Background>()->add("assets/Scenes/Setting/setting.png", 2, 2);

	BackButton.load(math::vec2{ 0.1, 0.1 }, "assets/UI/Buttons/Back.png");

	math::vec2 winSize = Engine::get_GE_component<IGL::Graphic>()->get_window_size();
	BGMSlider.load(winSize * math::vec2{ 0.4, 0.27 }, Engine::get_GE_component<Audio>()->get_bgm_sound(), Audio::BGMMaxValue);
	SFXSlider.load(winSize * math::vec2{ 0.4, 0.45 }, Engine::get_GE_component<Audio>()->get_effect_sound(), Audio::SFXMaxValue);
}

void Settings::update(double dt)
{
	BackButton.update(dt);
	BGMSlider.update_slider(Engine::get_GE_component<Audio>()->get_bgm_sound());
	SFXSlider.update_slider(Engine::get_GE_component<Audio>()->get_effect_sound());

	SceneManager* scm = Engine::get_GE_component<Game>()->get_game_component<SceneManager>();

	if (BackButton.is_button_pressed()) {
		scm->set_next_scene(Scenes::Mainmenu);
		scm->play_fading(SceneManager::MODE::FADE_IN, 0.2);
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Choose));
	}
}

void Settings::unload() {
	Engine::get_GE_component<Game>()->get_game_component<Background>()->clear();
	BackButton.unload();
	BGMSlider.unload();
	SFXSlider.unload();
}

void Settings::draw()
{
	Engine::get_GE_component<IGL::Graphic>()->set_background_color(0, 0, 0, 255);
	auto cam = Engine::get_GE_component<Game>()->get_game_component<Camera>();
	Engine::get_GE_component<Game>()->get_game_component<Background>()->draw(cam->get_matrix());

	BackButton.draw();
	BGMSlider.draw();
	SFXSlider.draw();
}
