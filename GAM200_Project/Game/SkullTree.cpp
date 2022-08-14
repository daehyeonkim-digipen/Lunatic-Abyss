#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Sprite.h"
#include "../Engine/Camera.h"
#include "../Engine/Collision.h"
#include "../Engine/Scene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Animation.h"
#include "../Engine/GameObjectManager.h"
#include "Particles.h"
#include "Hitbox.h"
#include "Gravity.h"
#include "Player.h"
#include "GOAttribute.h"

#include "SkullTree.h"

SkullTree::SkullTree(math::vec2 pos, GameObject* target)
	: GameObject(pos), targetObject(target)
{
	set_type(GOType::boss);
	set_name("Rotted GreatWood, The Gatekeeper of the Abyss");
	add_GO_component(new Sprite("assets/Sprite/Boss/SkullTree/sktree.spt", this));
	change_state(&stateIdle);
	add_GO_component(new GOAttribute(500, 5));

	explosionTimer = explosionCoolTime;
	rootTimer = rootCoolTime;
	thornTimer = thornCoolTime;
	nextAttack = AttackType::NORMAL;
}

void SkullTree::update(double dt) {

	if (isRotten) return;

	GameObject::update(dt);

	if (explosionTimer > 0)
		explosionTimer -= dt;
	if (rootTimer > 0)
		rootTimer -= dt;
	if (thornTimer > 0)
		thornTimer -= dt;
}

void SkullTree::draw(math::TransformMatrix viewportMatrix) {
	GameObject::draw(viewportMatrix);
}

void SkullTree::resolve_collision(GameObject*) {}

bool SkullTree::can_collide_with(GOType t) {
	return t == GOType::hitbox;
}

// === States ====================================================================

void SkullTree::SKTreeIdle::enter(GameObject* object) {
	SkullTree* boss = static_cast<SkullTree*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::IDLE));
	boss->set_timer(1.75 + rand() % 2000 / 1000);
}
void SkullTree::SKTreeIdle::update(GameObject*, double) {}
void SkullTree::SKTreeIdle::test_for_exit(GameObject* object) {
	SkullTree* boss = static_cast<SkullTree*>(object);

	auto objAttribute = boss->get_GO_components<GOAttribute>();
	if (objAttribute != nullptr && objAttribute->get_hp() <= 0) {
		boss->change_state(&boss->stateDead);
	}

	if (boss->is_timer_done() == true) {
		if (boss->explosionTimer <= 0 && abs(boss->targetObject->get_position().x - boss->get_position().x) < 130) {
			boss->nextAttack = SkullTree::AttackType::EXPLOSION;
			boss->change_state(&boss->stateAttack);
			boss->explosionTimer = boss->explosionCoolTime;
		}
		else if (abs(boss->targetObject->get_position().x - boss->get_position().x) < 200) {
			boss->nextAttack = SkullTree::AttackType::NORMAL;
			boss->change_state(&boss->stateAttack);
		}
		else if (boss->rootTimer <= 0) {
			boss->nextAttack = SkullTree::AttackType::ROOT;
			boss->change_state(&boss->stateAttack);
			boss->rootTimer = boss->rootCoolTime;
		}
		else if (boss->thornTimer <= 0) {
			boss->nextAttack = SkullTree::AttackType::THORN;
			boss->change_state(&boss->stateAttack);
			boss->thornTimer = boss->thornCoolTime;
		}

	}

}

void SkullTree::SKTreeAttack::enter(GameObject* object) {
	SkullTree* boss = static_cast<SkullTree*>(object);

	if (boss->nextAttack == SkullTree::AttackType::NORMAL) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STMove));
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::NORMAL));
		boss->set_timer(1.3);
	}
	else if (boss->nextAttack == SkullTree::AttackType::ROOT) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STMove));
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::ROOT));
		boss->set_timer(0.875);
	}
	else if (boss->nextAttack == SkullTree::AttackType::THORN) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STPreThrow));
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STThrowSound));
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::THORN));
		boss->set_timer(3);
	}
	else if (boss->nextAttack == SkullTree::AttackType::EXPLOSION) {
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STMove));
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::EXPLO));
		boss->set_timer(2);
	}
	soundSwitch = false;
	hasAttacked = false;
}
void SkullTree::SKTreeAttack::update(GameObject* object, double) {
	SkullTree* boss = static_cast<SkullTree*>(object);
	GameObjectManager* GOM = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>();
	double bossDamage = boss->get_GO_components<GOAttribute>()->get_ad();

	math::vec2 treeKnockBack = math::vec2{ -GameObject::DEFAULT_KNOCKBACK.x, GameObject::DEFAULT_KNOCKBACK.y };

	if(boss->nextAttack == SkullTree::AttackType::THORN && !soundSwitch && boss->is_timer_done())
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STThrowUp));
		soundSwitch = true;
		boss->set_timer(1);
	}

	if (boss->nextAttack == SkullTree::AttackType::EXPLOSION && !soundSwitch && boss->is_timer_done())
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STGather));
		soundSwitch = true;
		boss->set_timer(2);
	}

	if (boss->is_timer_done() && hasAttacked == false) {
		if (boss->nextAttack == SkullTree::AttackType::NORMAL) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STAttack));
			Hitbox* explosion = new Hitbox(boss, boss->get_position(), { {-230, 0}, {50, 230} }, bossDamage * 2.5, treeKnockBack * 1.7, 0.15);
			GOM->add_game_object(ObjType::E_Bullet, explosion);
		}
		else if (boss->nextAttack == SkullTree::AttackType::ROOT) {
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STGround));
			for (int i = 0; i < 12; i++) {
				const double xOffset = 80.0 * i;
				Hitbox* root = new Hitbox(boss, boss->get_position() - math::vec2{ 100 + xOffset, 0.0 }, { {-22, 0}, {22, 150} }, bossDamage, treeKnockBack * 0.4, 1, i % 2 == 0 ? 0 : 2.6, false, Hitbox::Debuff::count, "assets/Sprite/Particles/Root.spt", i < 2 ? Audio::Effects::STRoot : Audio::Effects::Count);
				GOM->add_game_object(ObjType::E_Bullet, root);
			}
		}
		else if (boss->nextAttack == SkullTree::AttackType::THORN) {
			for (int j = 0; j < 3; j++) {
				const double delayOffset = j * 2.5;
				for (int i = 0; i < 12; i++) {
					const double xOffset = 75.0 * i;
					Hitbox* thorn = new Hitbox(boss, boss->get_position() - math::vec2{ xOffset, 0.0 }, { {-15, 0}, {15, 15} }, bossDamage, treeKnockBack, 0.5, j + 0.25 * i, false, Hitbox::Debuff::count, "assets/Sprite/Particles/thorn.spt", Audio::Effects::STThrowMove, Audio::Effects::STTMHit);
					GOM->add_game_object(ObjType::E_Bullet, thorn);
				}
			}
		}
		else if (boss->nextAttack == SkullTree::AttackType::EXPLOSION) {
			Hitbox* explosion = new Hitbox(boss, boss->get_position(), { {-230, 0}, {0, 230} }, bossDamage * 2.5, treeKnockBack * 2.5, 0.7, 0.0, false, Hitbox::Debuff::count, "", Audio::Effects::FAAdvent);
			GOM->add_game_object(ObjType::E_Bullet, explosion);
		}

		hasAttacked = true;
	}
}
void SkullTree::SKTreeAttack::test_for_exit(GameObject* object) {
	SkullTree* boss = static_cast<SkullTree*>(object);
	
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() == true) {
		boss->change_state(&boss->stateIdle);
	}
}


void SkullTree::SKTreeDead::enter(GameObject* object) {
	SkullTree* boss = static_cast<SkullTree*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::DEAD));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::STDead));

	boss->remove_GO_component<RectCollision>();
	boss->remove_GO_component<GOAttribute>();
	boss->set_velocity({ 0, 0 });
}
void SkullTree::SKTreeDead::update(GameObject*, double) {}
void SkullTree::SKTreeDead::test_for_exit(GameObject* object) {
	SkullTree* boss = static_cast<SkullTree*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() == true) {
		boss->isRotten = true;
	}
}
