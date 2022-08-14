#pragma once
#include "../Engine/GameObject.h"

class SkullTree : public GameObject {
public:
	SkullTree(math::vec2 pos, GameObject* target);

	void update(double dt) override;
	void draw(math::TransformMatrix viewportMatrix) override;
	void resolve_collision(GameObject*) override;
	bool can_collide_with(GOType t) override;

private:
	GameObject* targetObject; 
	bool isRotten = false;

	enum class AttackType { NORMAL, EXPLOSION, ROOT, THORN } nextAttack;
	
	double explosionTimer = 0;
	double rootTimer = 0;
	double thornTimer = 0;

	const double explosionCoolTime = 15;
	const double rootCoolTime = 7;
	const double thornCoolTime = 10;

private:
	enum class Anim {
		IDLE, EXPLO, ROOT, THORN, NORMAL, DEAD
	};

	class SKTreeIdle : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};
	class SKTreeAttack : public State {
		bool hasAttacked = false;
		bool soundSwitch = false;
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class SKTreeDead : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	SKTreeIdle stateIdle;
	SKTreeAttack stateAttack;
	SKTreeDead stateDead;
};

