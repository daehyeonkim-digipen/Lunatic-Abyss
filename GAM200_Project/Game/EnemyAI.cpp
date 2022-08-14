#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Scene.h"
#include "Enemy.h"
#include "EnemyAI.h"
#include "Gravity.h"


EnemyAI::EnemyAI(AIType whichType, Enemy* owner) : aiType(whichType), owner(owner), player(Engine::get_GE_components<Game>()->get_game_components<GameObjectManager>()->get_game_object<Player>(ObjType::Player))
{
	switch(aiType)
	{
	case AIType::Lazy:
		patternChangeSpeed = 3;
		break;
	case AIType::Diligent:
		patternChangeSpeed = 1;
		break;
	default:
		patternChangeSpeed = 0;
		break;
	}
	owner->change_state(&owner->statePatrol);
}


void EnemyAI::update(double dt) {
	if (owner == nullptr)
	{
		Engine::get_logger().log_error("ai owner is nullptr");
	}
	if (const auto gravity = Engine::get_GE_components<Game>()->get_game_components<SceneManager>()->get_curr_scene()->get_GS_components<Gravity>()) {
		if (owner->currState != &owner->stateDead)
		{
			owner->update_velocity({ 0., -gravity->get_value() * dt });
		}
	}
}

