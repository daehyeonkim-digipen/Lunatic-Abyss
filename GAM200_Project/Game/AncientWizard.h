#pragma once
#include "../Engine/GameObject.h"

class AncientWizard : public GameObject {

public:
	AncientWizard(math::vec2 pos, Player* target);

	void update(double dt) override;
	void draw(math::TransformMatrix viewportMatrix) override;
	void resolve_collision(GameObject*) override;
	bool can_collide_with(GOType t) override;
	bool is_teleported();
	void set_teleported();
	bool is_ult();
private:
	Player* targetObject;
	GameObject* light;
	GameObject* blackhole;
	bool teleport = false;
	bool blindOn = false;
	bool ult = false;
	bool blackholeOn = false;
private:
	enum class Anim {
		IDLE, ATTACK, ATTACK2, MAGIC, BLACKHOLE, TELIN, TELOUT, DEAD
	};


	class AWIdle : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		double tpTimer = 3;
	};

	class AWBlind : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class AWIce : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class AWLaser : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		math::vec2 pos;
	};

	class AWAttack : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		bool soundSwitch = false;
	};

	class AWTeleport : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class AWBlackHole : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class AWDead : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	AWIdle stateIdle;
	AWBlind stateBlind;
	AWIce stateIce;
	AWLaser stateLaser;
	AWAttack stateAttack;
	AWTeleport stateTeleport;
	AWBlackHole stateBlackHole;
	AWDead stateDead;
};

