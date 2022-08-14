#include "RangeEnemyAI.h"
#include "Enemy.h"
#include "Player.h"
#include "../Engine/Sprite.h"
#include "../Engine/Audio.h"
#include <cmath>
RangeEnemyAI::RangeEnemyAI(AIType whichType, Enemy* owner, int sightRange, int runRange) : EnemyAI(whichType, owner), sightRange(sightRange), runRange(runRange) {}


 void RangeEnemyAI::exitPatrol()
{
	 bool enemyLookLeft = owner->get_scale().x >= 0;
	 if (owner->get_position().x - player->get_position().x < sightRange && enemyLookLeft || player->get_position().x - owner->get_position().x > sightRange && !enemyLookLeft)
	 {
		 owner->change_state(&owner->stateAttack);
		 patternTimer = 0;
	 }
}

void RangeEnemyAI::exitChase()
{
	Engine::get_logger().log_error("This state shouldn't be called");
}

void RangeEnemyAI::exitRun()
{
	 if (abs(owner->get_position().x - player->get_position().x) > runRange)
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

 void RangeEnemyAI::exitJump()
{
	 if (patternTimer > patternChangeSpeed / 10.)
	 {
		 owner->change_state(&owner->stateRun);
		 patternTimer = 0;
	 }
}

 void RangeEnemyAI::exitAttack()
{
	 if (abs(owner->get_position().x - player->get_position().x) < runRange)
	 {
		 owner->change_state(&owner->stateRun);
		 patternTimer = 0;
	 }
}

 void RangeEnemyAI::exitDamaged()
{
	 if (owner->get_GO_components<Sprite>()->is_animation_done())
	 {
		 owner->change_state(&owner->stateRun);
		 owner->get_GO_components<Sprite>()->set_damaged(false);
		 patternTimer = 0;
	 }
}