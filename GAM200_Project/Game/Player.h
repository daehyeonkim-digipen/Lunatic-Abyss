#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Input.h"

class Sprite;
class DropItem;
class CollisionBlock;
class Player : public GameObject {
	using GameKey = InputKey::Keyboard;
	using GameMouse = InputMouse::MouseButton;
	friend class HUD;
struct PlayerStatus
{
	//Common
	bool usedFTH2inThisStage = false;
	//buff
	double superArmorTimer = 0;
	bool superArmor = false;
	//Debuff
	double confusionTimer = 0;
	double blindTimer = 0;
	double freezeTimer = 0;

	bool blind;
	bool confusion;
	bool freeze;
	//Skills
	bool STR1 = false;
	bool STR2 = false;
	bool HLT1 = false;
	bool HLT2 = false;
	bool FTH1 = false;
	bool FTH2 = true;

} playerStatus;
private:
	//void save_data();
private: //data handling
	//bool hasLoaded = false;
public: //Basic Methods
	Player(math::vec2 pos);
	~Player()  override;
	GameObject* confusion;
	GameObject* blind;
	GameObject* superArmor;
	void update(double dt) override;
	void draw(math::TransformMatrix viewportMatrix) override;
	void resolve_collision(GameObject*) override;
	bool can_collide_with(GOType t) override;
private: //key
	InputKey moveLeftKey, moveRightKey, jumpKey, interactKey, chargeKey;
	InputMouse attackkey, guardKey;
private: //factors
	static constexpr double MOVEMENT_MAX_SPEED{ 600 / 2.0 };
	double animationSpeedFactor = 0.3;
	int attackPattern = 0;
	double patternTimer = 0;
public: //Player Behavior
	void update_moving(double dt);
	bool update_key_down(double dt);
	void update_fraction(double dt);
	bool is_player_bloking();
	PlayerStatus& get_ps();
private: //Player State
	enum class Anim {
		a1, a2, Sa, idle, running, falling, jumping, guard, charge, damaged, dead
	};

	class PlayerStateIdle : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateRunning : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateAttack : public State {
	private:
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
		bool repeat = false;
		bool check = false;
	};

	class PlayerStateSuperAttack : public PlayerStateAttack {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateJump : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateFalling : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateGuarding : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateSuperGuarding : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateCharging : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateDamaged : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};

	class PlayerStateDead : public State {
		void enter(GameObject*) override;
		void update(GameObject*, double) override;
		void test_for_exit(GameObject*) override;
	};


	PlayerStateIdle stateIdle;
	PlayerStateRunning stateRunning;
	PlayerStateAttack stateAttack;
	PlayerStateCharging stateCharging;
	PlayerStateSuperAttack stateSuperAttack;
	PlayerStateJump stateJump;
	PlayerStateFalling stateFalling;
	PlayerStateGuarding stateGuarding;
	PlayerStateSuperGuarding stateSuperGuarding;
	PlayerStateDamaged stateDamaged;
	PlayerStateDead stateDead;
};

