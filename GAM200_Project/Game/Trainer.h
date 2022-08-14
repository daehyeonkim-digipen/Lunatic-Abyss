#pragma once
#include "../Engine/GameObject.h"

class Trainer : public GameObject {
public:
	Trainer(math::vec2 pos, GameObject* target);

	void update(double dt) override;
	void draw(math::TransformMatrix viewportMatrix) override;
	void resolve_collision(GameObject*) override;
	bool can_collide_with(GOType t) override;

private:
	bool is_target_left();

	GameObject* targetObject; // usually player

private:
	enum class TrainerAnim {
		IDLE, GUARD, ATTACK
	};

	class TrainerStateIdle : public State {
		double idleTime;
		int shouldAttack;
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};
	class TrainerStateGuard : public State {
		double guardTime;
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};
	class TrainerStateAttack : public State {
		double attackIntervalTimer;

		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	TrainerStateIdle stateIdle;
	TrainerStateGuard stateGuard;
	TrainerStateAttack stateAttack;
};

