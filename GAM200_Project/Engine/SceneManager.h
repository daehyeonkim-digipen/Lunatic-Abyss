#pragma once
#include "Component.h"
#include "Scene.h"
#include "ShaderEffect.h"

#include <map>

class SceneManager : public Component {
	friend class Game;
public:
	SceneManager();
	void update(double deltaTime) override;
	bool has_scene_ended() { return currState == STATE::NONE; };
	void set_next_scene(Scenes sceneType);
	void add_game_scene(Scenes sceneType, Scene& gameScene);

	bool is_current_scene(Scenes st);
	Scene* get_curr_scene();

	enum class MODE { FADE_OUT, FADE_IN };

	void play_fading(MODE mode, double duration);
	void draw_fade_effect();
	bool is_fading_done();
private:
	ShaderEffect fadingEff;
	MODE sceneChangeMode;
	double totalEffTime;
	double effLifeTime;
	bool hadDoneFading;

	enum class STATE { START, LOAD, UNLOAD, UPDATE, EXIT, NONE } currState;

	std::map<Scenes, Scene*> mscenes;
	Scene* currScene;
	Scene* nextScene;
};