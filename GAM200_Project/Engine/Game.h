#pragma once
#include "Component.h"
#include "ComponentManager.h"
#include "SceneManager.h"
#include "Input.h"

class GameObject;
class Game : public Component {
	using GameKey = InputKey::Keyboard;
public:
	Game();
	~Game() override;
	void set_hitstop();
	template <typename T>
	T* get_game_component() { return gameComponentManager.get_component<T>(); }

	template <typename T>
	T* get_GS_component() { return gameComponentManager.get_component<SceneManager>()->currScene->get_GS_components<T>(); };

	void update(double) override;	
	void send_player_added() { hasPlayerAdded = true; };

	Scenes statNextScene = Scenes::SkullTreeStage;
private:
	double hitstopTimer = 0;
	ComponentManager gameComponentManager;

	bool hasPlayerAdded = false;
	bool hasPlayedFading = false;
};