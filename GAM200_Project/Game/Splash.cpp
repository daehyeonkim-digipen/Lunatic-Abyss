#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Graphic.h"
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Texture.h"

#include "Splash.h"

void Splash::load() {
	TextureManager* txm = Engine::get_GE_component<TextureManager>();

	splashes.push_back(txm->load("assets/Scenes/Splashes/DIGIPEN.png"));
	splashes.push_back(txm->load("assets/Scenes/Splashes/team.png"));

	eachSplashTimer = SPLASH_TIME;
	Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_IN, FADING_TIME);
}

void Splash::unload() {
	TextureManager* txm = Engine::get_GE_component<TextureManager>();
	for (Texture* texx : splashes)
		txm->remove_texture(texx);
}

void Splash::update(double dt) {

#ifdef _DEBUG
	if (Engine::get_GE_component<Input>()->is_key_pressed(InputKey::Keyboard::Enter) == true) {
		Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::Mainmenu);
	}
#endif // DEBUG

	if (eachSplashTimer > 0) {
		eachSplashTimer -= dt;

		if (eachSplashTimer <= FADING_TIME && hasPlayedFading == false) {
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_OUT, FADING_TIME);
			hasPlayedFading = true;
		}

		if (eachSplashTimer < 0) {
			if (++currSplashIndex == splashes.size()) {
				Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->set_next_scene(Scenes::Mainmenu);
			}

			eachSplashTimer = SPLASH_TIME;
			Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->play_fading(SceneManager::MODE::FADE_IN, FADING_TIME);
			hasPlayedFading = false;
		}
	}

}

void Splash::draw() {
	Engine::get_GE_component<IGL::Graphic>()->set_background_color(0, 0, 0, 1);

	if (currSplashIndex == splashes.size()) return;

	math::vec2 splashSize = splashes[currSplashIndex]->get_size();
	math::vec2 windowSize = Engine::get_GE_component<IGL::Graphic>()->get_window_size();

	splashes[currSplashIndex]->draw(math::TranslateMatrix{ windowSize / 2 - splashSize / 2 });
}
