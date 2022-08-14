#pragma once
#include "../Engine/Component.h"
class Player;
class EnemyAI : public Component
{
	friend class Enemy;
public:
	enum class AIType
	{
		Foolish,
		Lazy,
		Diligent,
		Smart,
	};

public:
	EnemyAI(AIType, Enemy*);
	void update(double dt) override;

protected:
	Player* player;
	Enemy* owner;
protected:
	AIType aiType;
	double patternChangeSpeed;
	double patternTimer;
public:
	virtual void exitPatrol() = 0;
	virtual void exitChase() = 0;
	virtual void exitRun() = 0;
	virtual void exitJump() = 0;
	virtual void exitAttack() = 0;
	virtual void exitDamaged() = 0;
};