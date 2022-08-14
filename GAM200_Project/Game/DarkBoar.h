#pragma once
#include "../Engine/GameObject.h"

class Hitbox;
class DarkBoar : public GameObject
{
public:
	DarkBoar(math::vec2 pos, GameObject* target);

	void update(double dt) override;
	void draw(math::TransformMatrix viewportMatrix) override;
	void resolve_collision(GameObject*) override;
	bool can_collide_with(GOType t) override;

private:
	bool is_target_left();
	bool is_target_back();


	static constexpr double moveSpeed = 100;

	GameObject* targetObject; // usually player
	bool isAngry = false;

	std::string secondPhaseName;

private:
	enum class BoarAnim { 
		IDLE, MOV, RUSH, BITE, PUSH, KICK, TRANSFORM 
	};

	enum class ManAnim { 
		APPEAR, IDLE, MOV, SWORD1, SWORD2, BATTLECRY, SLIDE, DEAD 
	};

	enum class AttackPattern { 
		BITE, KICK, RUSH, PUSH, SWORD1, SWORD2, BATTLECRY, SLIDE, NONE
	} currAttackPattern = AttackPattern::NONE;


	class TBossStateIdle : public State {
		double idleTime;
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};
	class TBossStateMove: public State {
		double patrolTime;
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};
	class TBossStateAttack : public State {
		double attackIntervalTimer;
		bool hitboxInsertTrigger;

		double rushDuration;
		GameObject* rushHitbox;

		bool soundSwitch;
		bool secondAttackTrigger;
		int undeadShoutingCount;

		double slideDuration;
		double slideVelocityX;
		GameObject* slideCutHitbox;

		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;

	};
	class TBossTransform : public State {
		double transformAttackTimer;
		bool secondAttackTrigger;
		math::vec2 enteredPosition;
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};
	class TBossDead : public State {
		bool transformTrigger = false;
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};


	TBossStateIdle stateIdle;
	TBossStateMove stateMove;
	TBossStateAttack stateAttack;
	TBossTransform stateTransform;
	TBossDead stateDead;
};

