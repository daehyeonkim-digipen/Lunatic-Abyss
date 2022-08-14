#pragma once
#include "Engine.h"
#include "Input.h"

#include "Scene.h"
#include "SceneManager.h"


SceneManager::SceneManager()
	: currScene(nullptr), nextScene(nullptr), currState(STATE::START)
{
	fadingEff = ShaderEffect(CustomShader("assets/Shaders/e_default.vert", "assets/Shaders/e_fading_out.frag"), Engine::get_GE_component<IGL::Graphic>()->get_window_size());
}

void SceneManager::update(double deltaTime) {
	switch (currState)
	{
	case STATE::START:
		if (mscenes.size() == 0) {
			currState = STATE::EXIT;
		}
		else {
			nextScene = mscenes[Scenes::Splash];
			currState = STATE::LOAD;
		}
		break;
	case STATE::LOAD:
		currScene = nextScene;
		currScene->load();
		currState = STATE::UPDATE;
		break;
	case STATE::UPDATE:
		currScene->update(deltaTime);
		currScene->draw();
		if (currScene != nextScene) {
			currState = STATE::UNLOAD;
		}
		break;
	case STATE::UNLOAD:
		currScene->unload();
		if (nextScene == nullptr)
			currState = STATE::EXIT;
		else {
			currScene = nextScene;
			currState = STATE::LOAD;
		}
		break;
	case STATE::EXIT:
		currState = STATE::NONE;
		break;
	case STATE::NONE:
		break;
	default:
		break;
	}

	if (hadDoneFading == false) {
		switch (sceneChangeMode)
		{
		case SceneManager::MODE::FADE_OUT:
			effLifeTime += deltaTime;
			if (effLifeTime >= totalEffTime) {
				hadDoneFading = true;
			}
			break;
		case SceneManager::MODE::FADE_IN:
			effLifeTime -= deltaTime;
			if (effLifeTime <= 0) {
				hadDoneFading = true;
			}
			break;
		default:
			hadDoneFading = true;
			break;
		}
	}
}

void SceneManager::set_next_scene(Scenes sceneType) {
	sceneType == Scenes::Exit ? nextScene = nullptr : nextScene = mscenes[sceneType];
}

void SceneManager::add_game_scene(Scenes sceneType, Scene& gameScene) {
	mscenes.insert(std::pair<Scenes, Scene*>(sceneType, &gameScene));
}

bool SceneManager::is_current_scene(Scenes st) {
	return currScene == mscenes.find(st)->second;
}

Scene* SceneManager::get_curr_scene()
{
	return currScene;
}

void SceneManager::play_fading(SceneManager::MODE mode, double duration) {
	sceneChangeMode = mode;
	totalEffTime = duration;
	hadDoneFading = false;

	switch (mode)
	{
	case SceneManager::MODE::FADE_OUT:
		effLifeTime = 0;
		break;
	case SceneManager::MODE::FADE_IN: // -> becoming bright -> alpha should be decreased -> initial alpha value should be bigger
		effLifeTime = totalEffTime;
		break;
	default:
		break;
	}
}

void SceneManager::draw_fade_effect() {
	// if (is_fading_done() == true) return;

	fadingEff.set_variable(Uniforms::LifeTime, static_cast<float>(effLifeTime / totalEffTime));
	fadingEff.draw({});
}

bool SceneManager::is_fading_done() {
	return hadDoneFading;
}


