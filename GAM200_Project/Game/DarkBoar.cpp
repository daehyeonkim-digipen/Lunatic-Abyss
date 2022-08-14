#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Sprite.h"
#include "../Engine/Camera.h"
#include "../Engine/Collision.h"
#include "../Engine/GameObjectManager.h"
#include "Hitbox.h"
#include "Gravity.h"
#include "GOAttribute.h"
#include "DarkBoar.h"

DarkBoar::DarkBoar(math::vec2 pos, GameObject* target)
	: GameObject(pos), targetObject(target)
{
	set_type(GOType::boss);
	set_name("Dark Boar, The Collection from the Abyss");
	add_GO_component(new Sprite("assets/Sprite/Boss/DarkBoar/Phase1/sprite.spt", this));
	change_state(&stateIdle);
	set_scale({ -1, 1 });
	add_GO_component(new GOAttribute(250, 20));
	secondPhaseName = "The Undead, The Collection from the Abyss";
}

void DarkBoar::update(double dt)
{
	GameObject::update(dt);
	Gravity* gravity = Engine::get_GE_component<Game>()->get_GS_component<Gravity>();
	if (gravity != nullptr) {
		if (currState != &stateDead) {
			update_velocity(math::vec2{ 0.0, -gravity->get_value() * dt });
		}
	}

	auto objAttribute = get_GO_components<GOAttribute>();
	if (objAttribute != nullptr && objAttribute->get_hp() <= 0 && currState != &stateTransform) {
		change_state(&stateDead);
	}
}

void DarkBoar::draw(math::TransformMatrix viewportMatrix) {
	GameObject::draw(viewportMatrix);
}

void DarkBoar::resolve_collision(GameObject* object) {
	switch (object->get_type())
	{
	case GOType::hitbox:
		break;
	default:
		break;
	}
}

bool DarkBoar::can_collide_with(GOType t) {
	switch (t)
	{
	case GameObject::GOType::hitbox:
		return true;
	default:
		return false;
	}
}

bool DarkBoar::is_target_left() {
	return targetObject->get_position().x < get_position().x;;
}

bool DarkBoar::is_target_back() {
	bool isBossHeadingRight = 0 < get_scale().x;
	return is_target_left() == isBossHeadingRight;
}

// === States ====================================================================

void DarkBoar::TBossStateIdle::enter(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	boss->get_GO_components<Sprite>()->play_animation(boss->isAngry ? static_cast<int>(ManAnim::IDLE) : static_cast<int>(BoarAnim::IDLE));
	boss->set_velocity({ 0.0, boss->get_velocity().y });
	idleTime = 0.5 + static_cast<double>(rand() % 300) / 100.0; // 0.5 to 3 with real number
	Engine::get_logger().log_debug("IDLE : " + std::to_string(idleTime));
}
void DarkBoar::TBossStateIdle::update(GameObject*, double dt) {
	if (idleTime > 0)
		idleTime -= dt;
}
void DarkBoar::TBossStateIdle::test_for_exit(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	double distanceX = boss->get_position().x - boss->targetObject->get_position().x;

	if (idleTime < 0) {
		if (boss->isAngry == false) {
			if (std::abs(distanceX) < 30) {
				if (boss->is_timer_done() && !boss->isAngry)
				{
					int r = rand() % 3;
					Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBRoar1) + r);
					boss->set_timer(3);
				}
				boss->currAttackPattern = AttackPattern::PUSH;
				boss->change_state(&boss->stateAttack);
			}
			else if (std::abs(distanceX) < 130) {
				if (boss->is_timer_done() && !boss->isAngry)
				{
					int r = rand() % 3;
					Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBRoar1) + r);
					boss->set_timer(3);
				}
				boss->currAttackPattern = boss->is_target_back() ? AttackPattern::KICK : AttackPattern::BITE;
				boss->change_state(&boss->stateAttack);
			}
			else if (std::abs(distanceX) > 260) {
				if(!boss->isAngry)
				{
					int r = rand() % 3;
					Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBRoar1) + r);
					boss->set_timer(3);
				}
				boss->currAttackPattern = AttackPattern::RUSH;
				boss->change_state(&boss->stateAttack);
			}
			else {
				if (boss->is_timer_done() && !boss->isAngry)
				{
					int r = rand() % 3;
					Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBRoar1) + r);
					boss->set_timer(3);
				}
				boss->change_state(&boss->stateMove);
			}
		}
		else {
			boss->change_state(&boss->stateMove);
		}
	}

}

void DarkBoar::TBossStateMove::enter(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	boss->get_GO_components<Sprite>()->play_animation(boss->isAngry ? static_cast<int>(ManAnim::MOV) : static_cast<int>(BoarAnim::MOV));

	patrolTime = static_cast<double>(rand() % 500) / 100.0; // 0 to 5 with real number
	Engine::get_logger().log_debug("MOVE : " + std::to_string(patrolTime));

	const auto r = rand() % 3;
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBRoar4) + r);
}
void DarkBoar::TBossStateMove::update(GameObject* object, double dt) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	bool isOverriding = std::abs(boss->targetObject->get_position().x - boss->get_position().x) < 1;

	if (isOverriding == false)
		boss->set_scale({ boss->is_target_left() ? -1 : 1, 1});
	boss->set_velocity({ boss->is_target_left() ? -DarkBoar::moveSpeed * dt : DarkBoar::moveSpeed * dt, 0.0 });

	if (patrolTime > 0)
		patrolTime -= dt;
}
void DarkBoar::TBossStateMove::test_for_exit(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	double distanceX = boss->get_position().x - boss->targetObject->get_position().x;

	if (patrolTime > 0) {
		if (boss->isAngry == false) {
			if (std::abs(distanceX) < 110)
				boss->change_state(&boss->stateIdle);
		}
		else {
			if (boss->isAngry == true) {
				if (std::abs(distanceX) < 70) {
					Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBPreScream));

					boss->currAttackPattern = AttackPattern::BATTLECRY;
					boss->change_state(&boss->stateAttack);
				}
				else if (std::abs(distanceX) < 200) {
					std::mt19937 generator((unsigned int)time(NULL));
					std::uniform_int_distribution<int> distributor(static_cast<int>(AttackPattern::SWORD1), static_cast<int>(AttackPattern::SWORD2));
					boss->currAttackPattern = static_cast<AttackPattern>(distributor(generator));
					boss->change_state(&boss->stateAttack);
				}
				else if (std::abs(distanceX) > 300) {
					boss->currAttackPattern = AttackPattern::SLIDE;
					boss->change_state(&boss->stateAttack);
				}
			}
		}
	}
	else {
		boss->change_state(&boss->stateIdle);
	}

}

void DarkBoar::TBossTransform::enter(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	boss->get_GO_components<Sprite>()->play_animation(boss->isAngry ? static_cast<int>(ManAnim::APPEAR) : static_cast<int>(BoarAnim::TRANSFORM));
	if (boss->isAngry != true) {
		boss->remove_GO_component<RectCollision>(); // to prevent attacking while transforming
	}
	else {
		transformAttackTimer = 1.795;
		secondAttackTrigger = false;
	}
	enteredPosition = boss->get_position();
}
void DarkBoar::TBossTransform::update(GameObject* object, double dt) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	boss->set_position(enteredPosition);
	boss->set_velocity({ 0, 0 }); // totally fixed position!

	GameObjectManager* GOM = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>();
	Camera* cam = Engine::get_GE_component<Game>()->get_game_component<Camera>();

	if (transformAttackTimer > 0) {
		transformAttackTimer -= dt;
		if (transformAttackTimer < 0) {
			transformAttackTimer = 0;
		}
	}

	if (transformAttackTimer != 0 || boss->isAngry == false) return;

	double bossDamage = boss->get_GO_components<GOAttribute>()->get_ad();
	math::vec2 knockbackVector = math::vec2{ boss->get_scale().x, 1.0 } *GameObject::DEFAULT_KNOCKBACK;
	knockbackVector.x *= boss->is_target_back() ? -1 : 1;

	if (secondAttackTrigger == false) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBAttack2));
		Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -130, 0 }, { 50, 260 } }, bossDamage, knockbackVector);
		GOM->add_game_object(ObjType::E_Bullet, hitbox);
		transformAttackTimer = 1.92;
		secondAttackTrigger = true;
	}
	else {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAStrongAttack));
		Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -120, 0 }, { 100, 260 } }, bossDamage, knockbackVector);
		GOM->add_game_object(ObjType::E_Bullet, hitbox);
		cam->shake({ 0, 25 }, 0.3, 0.02);
		transformAttackTimer = -1;
	}
}
void DarkBoar::TBossTransform::test_for_exit(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done()) {
		if (boss->isAngry == false) {
			boss->clear_GO_component();
			boss->add_GO_component(new Sprite("assets/Sprite/Boss/DarkBoar/Phase2/sprite.spt", boss));
			boss->add_GO_component(new GOAttribute(250, 25));
			boss->set_name(boss->secondPhaseName);
			boss->isAngry = true;
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBMutate));

			boss->change_state(&boss->stateTransform);
		}
		else {
			boss->change_state(&boss->stateIdle);
		}
	}
}

void DarkBoar::TBossDead::enter(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	if (boss->isAngry == false) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBPremutate));

		boss->change_state(&boss->stateTransform);
		return;
	}
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBPredie));

	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(ManAnim::DEAD));

	boss->set_velocity({ 0, 0 });
	boss->remove_GO_component<RectCollision>();
	boss->remove_GO_component<GOAttribute>();
	boss->set_timer(4.2);
	stateSwitch = false;
}
void DarkBoar::TBossDead::update(GameObject* object, double)
{
	DarkBoar* boss = static_cast<DarkBoar*>(object);

	if (!stateSwitch && boss->is_timer_done())
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBDie));
		stateSwitch = true;
	}
}
void DarkBoar::TBossDead::test_for_exit(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);

	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() == true) {
		boss->set_is_dead(true);
	}
}

void DarkBoar::TBossStateAttack::enter(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	stateSwitch = false;
	double bossAD = boss->get_GO_components<GOAttribute>()->get_ad();
	math::vec2 rushKnockback = math::vec2{ boss->get_scale().x, 1.0 } *GameObject::DEFAULT_KNOCKBACK;

	boss->set_velocity({ 0.0, boss->get_velocity().y });

	switch (boss->currAttackPattern)
	{
	case AttackPattern::BITE:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(BoarAnim::BITE));
		boss->update_position({ boss->get_scale().x * -40, 0.0 });
		attackIntervalTimer = 0.7;
		break;
	case AttackPattern::KICK:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(BoarAnim::KICK));
		attackIntervalTimer = 0.6;
		break;
	case AttackPattern::RUSH:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(BoarAnim::RUSH));
		boss->set_scale({ boss->is_target_left() ? -1 : 1, 1});
		attackIntervalTimer = 0.8;
		rushDuration = 0.2;
		hitboxInsertTrigger = false;

		rushHitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -70, 0 }, math::vec2{ 70, 100 } }, bossAD, rushKnockback, rushDuration + 1);
		break;
	case AttackPattern::PUSH:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(BoarAnim::PUSH));
		attackIntervalTimer = 0.5;
		break;
	case AttackPattern::SWORD1:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(ManAnim::SWORD1));
		boss->set_scale({ boss->is_target_left() ? -1 : 1, 1 });
		attackIntervalTimer = 1.25;
		break;
	case AttackPattern::SWORD2:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(ManAnim::SWORD2));
		boss->set_scale({ boss->is_target_left() ? -1 : 1, 1 });
		attackIntervalTimer = 1.25;
		secondAttackTrigger = false;
		break;
	case AttackPattern::BATTLECRY:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(ManAnim::BATTLECRY));
		attackIntervalTimer = 2.4;
		undeadShoutingCount = 4;
		boss->set_timer(2.4);
		break;
	case AttackPattern::SLIDE:
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(ManAnim::SLIDE));
		boss->set_scale({ boss->is_target_left() ? -1 : 1, 1 });
		attackIntervalTimer = 1.4;
		slideDuration = 0.45;
		slideVelocityX = (boss->targetObject->get_position().x - boss->get_position().x) / (slideDuration * 0.7);
		hitboxInsertTrigger = false;

		slideCutHitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -70, 0 }, math::vec2{ 70, 10 } }, bossAD, rushKnockback, rushDuration + 1);
		break;
	default:
		boss->change_state(&boss->stateIdle);
		return;
	}
	soundSwitch = false;

}
void DarkBoar::TBossStateAttack::update(GameObject* object, double dt) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	GameObjectManager* GOM = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>();
	Camera* cam = Engine::get_GE_component<Game>()->get_game_component<Camera>();

	if (attackIntervalTimer > 0) {
		attackIntervalTimer -= dt;
		if (attackIntervalTimer < 0) {
			attackIntervalTimer = 0; // let this marker
		}
	}

	if(boss->currAttackPattern == AttackPattern::SLIDE && attackIntervalTimer < .8 && !soundSwitch)
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBSwing));
		soundSwitch = true;
	}

	if (attackIntervalTimer == 0) { // anticipation 
		if (boss->currAttackPattern == AttackPattern::RUSH) {
			if (hitboxInsertTrigger == false) {
				Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBDash));

				GOM->add_game_object(ObjType::E_Bullet, rushHitbox);
				hitboxInsertTrigger = true;
			}

			double rushVelocity = boss->get_scale().x * 8 * DarkBoar::moveSpeed * dt;

			if (rushDuration > 0) { // now its moving 
				boss->set_velocity({ rushVelocity, 0.0 });
				rushDuration -= dt;
			}
			else {
				boss->set_velocity({ rushVelocity * 0.5, boss->get_velocity().y });
			}

			rushHitbox->set_position(boss->get_position());
		}
		else if (boss->currAttackPattern == AttackPattern::SLIDE) {
			if (hitboxInsertTrigger == false) {

				GOM->add_game_object(ObjType::E_Bullet, slideCutHitbox);
				hitboxInsertTrigger = true;
			}

			if (slideDuration > 0) {
				boss->set_velocity({ slideVelocityX * dt, 0.0 });
				slideDuration -= dt;
			}
			else {
				boss->set_scale({ boss->is_target_left() ? -1 : 1, 1 });
				boss->set_velocity({ 0, 0 });
			}

			slideCutHitbox->set_position(boss->get_position());
		}
	}

	if (attackIntervalTimer != 0) return;

	double bossDamage = boss->get_GO_components<GOAttribute>()->get_ad();
	math::vec2 knockbackVector = math::vec2{ boss->get_scale().x, 1.0 } * GameObject::DEFAULT_KNOCKBACK;

	if (boss->currAttackPattern == AttackPattern::BITE) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBBite));
		boss->update_position({ boss->get_scale().x * 90, 0.0 });
		Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -5, 0 }, { 115, 100 } }, bossDamage, knockbackVector);
		GOM->add_game_object(ObjType::E_Bullet, hitbox);
		attackIntervalTimer = -1;
	}
	else if (boss->currAttackPattern == AttackPattern::KICK) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBKick));

		boss->update_position({ -boss->get_scale().x * 50, 0.0 });
		Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -130, 0 }, { 10, 100 } }, bossDamage, knockbackVector * math::vec2{ -1, 0 });
		GOM->add_game_object(ObjType::E_Bullet, hitbox);
		attackIntervalTimer = -1;
	}
	else if (boss->currAttackPattern == AttackPattern::PUSH) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBAttack));

		Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -30, 0 }, { 10, 100 } }, 3, { -boss->get_scale().x * 6, 2.0 });
		GOM->add_game_object(ObjType::E_Bullet, hitbox);
		attackIntervalTimer = -1;
	}
	else if (boss->currAttackPattern == AttackPattern::SWORD1) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBAttack1));

		Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { 30, 0 }, { 200, 150 } }, bossDamage, knockbackVector);
		GOM->add_game_object(ObjType::E_Bullet, hitbox);
		cam->shake({ 0, 25 }, 0.3, 0.02);

		attackIntervalTimer = -1;
	}
	else if (boss->currAttackPattern == AttackPattern::SWORD2) {
		if (secondAttackTrigger == false) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBAttack1));

			Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { 30, 0 }, { 200, 150 } }, bossDamage, knockbackVector);
			GOM->add_game_object(ObjType::E_Bullet, hitbox);
			cam->shake({ 0, 25 }, 0.3, 0.02);

			attackIntervalTimer = 2.5;
			secondAttackTrigger = true;
		}
		else {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBAttack2));

			knockbackVector.x *= boss->is_target_back() ? -1 : 1;
			Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -150, 0 }, { 200, 200 } }, bossDamage, knockbackVector);
			GOM->add_game_object(ObjType::E_Bullet, hitbox);
			cam->shake({ 0, 25 }, 0.3, 0.02);

			attackIntervalTimer = -1;
		}
	}
	else if (boss->currAttackPattern == AttackPattern::BATTLECRY) {
		knockbackVector.x *= boss->is_target_back() ? -1 : 1;
		if (boss->is_timer_done() && !stateSwitch)
		{
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::DBScream));
			stateSwitch = true;
		}
		Hitbox* hitbox = new Hitbox(boss, boss->get_position(),math::rect2{ { -200, 0 }, { 200, 350 } }, bossDamage * 0.33, knockbackVector);
		GOM->add_game_object(ObjType::E_Bullet, hitbox);
		cam->shake({ 0, 20 }, 0.8, 0.02);

		attackIntervalTimer = --undeadShoutingCount > 0 ? 0.8 : -1;
	}

}
void DarkBoar::TBossStateAttack::test_for_exit(GameObject* object) {
	DarkBoar* boss = static_cast<DarkBoar*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done()) {
		boss->currAttackPattern = AttackPattern::NONE; // initialize
		boss->change_state(&boss->stateIdle);
	}
}
