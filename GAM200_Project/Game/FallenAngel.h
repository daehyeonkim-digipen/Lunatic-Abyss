#pragma once
#include "../Engine/GameObject.h"

class FallenAngel : public GameObject {
public:
	FallenAngel(math::vec2 pos, GameObject* target);
	enum class Phase { phase1, phase2 };

	void update(double dt) override;
	void draw(math::TransformMatrix viewportMatrix) override;
	void resolve_collision(GameObject*) override;
	bool can_collide_with(GOType t) override;
	Phase get_phase();
private:
	GameObject* targetObject;
	Phase phase;
	Sprite* phase2Sprite;
private:
	enum class Anim {
		IDLE, LASERL, LASERR, WING, CRACK
	};

	enum class Anim2 {
		IDLE, JUMP, DESCENT, WARPIN, WARPOUT, ATTACK,  DEAD
	};

	class FAIdle : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FALaserL : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FALaserR : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FAWingAttack : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FACrack : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FAFalling : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		math::vec2 cameraFixPosition;
	};

	class FAAdvent : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		math::vec2 adventPos;
	};

	class FAJump : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FADescent : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		bool soundSwitch = false;
	};

	class FAShockwave : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FAWarpIn : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FAWarpAttack : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class FAAttack : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		bool soundSwitch = false;
	};


	class FADead : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	FAIdle stateIdle;
	FALaserL stateLaserL;
	FALaserR stateLaserR;
	FAWingAttack stateWingAttack;
	FACrack stateCrack;
	FAFalling stateFalling;
	FAAdvent stateAdvent;

	FAJump stateJump;
	FADescent stateDescent;
	FAShockwave stateShockwave;
	FAWarpIn stateWarpIn;
	FAWarpAttack stateWarpAttack;
	FAAttack stateAttack;
	FADead stateDead;
};

