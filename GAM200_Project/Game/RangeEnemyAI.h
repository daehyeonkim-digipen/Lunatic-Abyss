#pragma once
#include "EnemyAI.h"
class RangeEnemyAI : public EnemyAI
{
public:
	RangeEnemyAI(AIType whichType, Enemy* owner, int sightRange, int runRange);
	void exitPatrol() override;
	void exitChase() override;
	void exitRun() override;
	void exitJump() override;
	void exitAttack() override;
	void exitDamaged() override;
private:
	int sightRange;
	int runRange;
};


