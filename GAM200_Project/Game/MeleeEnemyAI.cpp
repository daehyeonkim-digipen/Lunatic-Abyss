#include "../Engine/Sprite.h"
#include "../Engine/Audio.h"
#include "MeleeEnemyAI.h"
#include "Enemy.h"
#include "Player.h"

#include <cmath>

MeleeEnemyAI::MeleeEnemyAI(AIType whichType, Enemy* owner, int sightRange, int attackRange) : EnemyAI(whichType, owner), sightRange(sightRange), attackRange(attackRange) {}

void MeleeEnemyAI::exitPatrol()
{
	bool enemyLookLeft = owner->get_scale().x >= 0;
	if (owner->get_position().x - player->get_position().x < sightRange && enemyLookLeft || player->get_position().x - owner->get_position().x > sightRange && !enemyLookLeft)
	{
		owner->change_state(&owner->stateChase);
		patternTimer = 0;
	}
}

void MeleeEnemyAI::exitChase()
{
	if (abs(owner->get_position().x - player->get_position().x) < attackRange)
	{
		owner->change_state(&owner->stateAttack);
		patternTimer = 0;
	}
	else
	{
		if (patternTimer > patternChangeSpeed / 2.)
		{
			owner->change_state(&owner->stateJump);
			patternTimer = 0;

		}
	}
}

void MeleeEnemyAI::exitRun()
{
	Engine::get_logger().log_error("This state shouldn't be called");
}

void MeleeEnemyAI::exitJump()
{
	if (patternTimer > patternChangeSpeed / 10.)
	{
		owner->change_state(&owner->stateChase);
		patternTimer = 0;
	}
}

void MeleeEnemyAI::exitAttack()
{
	if (abs(owner->get_position().x - player->get_position().x) > attackRange)
	{
		owner->change_state(&owner->stateChase);
		patternTimer = 0;
	}
	else
	{
		if (owner->get_GO_components<Sprite>()->is_animation_done())
		{
			owner->change_state(&owner->stateAttack);
			patternTimer = 0;
		}
	}
}

void MeleeEnemyAI::exitDamaged()
{
	if (owner->get_GO_components<Sprite>()->is_animation_done())
	{
		owner->change_state(&owner->stateChase);
		owner->get_GO_components<Sprite>()->set_damaged(false);

		patternTimer = 0;
	}
}



