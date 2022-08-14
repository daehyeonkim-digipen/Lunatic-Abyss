#pragma once
#include <vector>
#include "../Engine/ComponentManager.h"

enum class Scenes {
	Splash,
	Exit,
	Mainmenu,
	Setting,

	StatStage,
	Tutorial,
	DarkBoarStage,
	SkullTreeStage,
	FallenAngelStage,
	AncientWizardStage
};

class GameObject;
class Scene {
	friend class GameMap;
public:
	Scene() = default;

	template <typename T>
	T* get_GS_components() { return gameSceneComponentManager.get_component<T>(); }

	virtual void load() = 0;
	virtual void unload() = 0;
	virtual void draw() = 0;
	virtual void update(double deltaTime) = 0;
protected:
	ComponentManager gameSceneComponentManager;
};
