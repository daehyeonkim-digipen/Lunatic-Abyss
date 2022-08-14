#pragma once
#include "EnemyAI.h"
class MeleeEnemyAI : public EnemyAI
{
public:
	MeleeEnemyAI(AIType whichType, Enemy* owner, int sightRange, int attackRange);
	void exitPatrol() override;
	void exitChase() override;
	void exitRun() override;
	void exitJump() override;
	void exitAttack() override;
	void exitDamaged() override;
private:
	int sightRange;
	int attackRange;
};
