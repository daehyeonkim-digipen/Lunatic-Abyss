#define CRTDBG_MAP_ALLOC

#include <stdexcept>	//std::exception
#include <iostream>		//std::cerr
#include "Engine/Engine.h"
#include "Engine/Game.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"
#include "Game/AWStage.h"


#include "Game/Splash.h"
#include "Game/MainMenu.h"
#include "Game/Settings.h"
#include "Game/StatScene.h"
#include "Game/TutorialStage.h"
#include "Game/DBstage.h"
#include "Game/STstage.h"
#include "Game/FAStage.h"

int main(void) try
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Engine& engine = Engine::instance();
	engine.init("Lunatic Abyss");

	Splash splash;
	MainMenu mainmenu;
	Settings settings;

	StatScene statStage;
	TutorialStage tutorial;
	DarkBoarStage tutorialBossStage;
	SkullTreeStage boss1Stage;
	FAStage faStage;
	AWStage awStage;
	// ==== scene setting === 

	SceneManager* scm = engine.get_GE_component<Game>()->get_game_component<SceneManager>();

	scm->add_game_scene(Scenes::Splash, splash);
	scm->add_game_scene(Scenes::Mainmenu, mainmenu);
	scm->add_game_scene(Scenes::Setting, settings);

	scm->add_game_scene(Scenes::StatStage, statStage);
	scm->add_game_scene(Scenes::Tutorial, tutorial);
	scm->add_game_scene(Scenes::DarkBoarStage, tutorialBossStage);
	scm->add_game_scene(Scenes::SkullTreeStage, boss1Stage);
	scm->add_game_scene(Scenes::FallenAngelStage, faStage);
	scm->add_game_scene(Scenes::AncientWizardStage, awStage);


	while (engine.has_game_ended() == false) {
		engine.update();
	}

	return 0;
}
catch (std::exception& e) // 
{
	std::cerr << e.what() << "\n";
	return -1;
}
