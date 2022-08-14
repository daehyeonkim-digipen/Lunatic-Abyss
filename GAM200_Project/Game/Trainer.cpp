#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Sprite.h"
#include "../Engine/GameObjectManager.h"
#include "Hitbox.h"
#include "Gravity.h"
#include "GOAttribute.h"

#include "Trainer.h"


Trainer::Trainer(math::vec2 pos, GameObject* target)
	: GameObject(pos), targetObject(target)
{
	set_type(GOType::boss);
	set_name("Cathedral Knight, The Guide of Knights");
	add_GO_component(new Sprite("assets/Sprite/Boss/Trainer/Trainer.spt", this));
	change_state(&stateIdle);
	set_scale({ -1, 1 });
	add_GO_component(new GOAttribute(250, 1));
}

void Trainer::update(double dt) {
	GameObject::update(dt);

	Gravity* gravity = Engine::get_GE_component<Game>()->get_GS_component<Gravity>();
	if (gravity != nullptr) {
		update_velocity(math::vec2{ 0.0, -gravity->get_value() * dt });
	}
}

void Trainer::draw(math::TransformMatrix viewportMatrix) {
	GameObject::draw(viewportMatrix);
}

void Trainer::resolve_collision(GameObject*) {}

bool Trainer::can_collide_with(GOType t) {
	return t == GameObject::GOType::hitbox;
}

bool Trainer::is_target_left() {
	return targetObject->get_position().x < get_position().x;
}

// === States ====================================================================

void Trainer::TrainerStateIdle::enter(GameObject* object) {
	Trainer* boss = static_cast<Trainer*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(TrainerAnim::IDLE));

	std::mt19937 generator((unsigned int)time(NULL));
	std::uniform_real_distribution<double> distributor(2, 3); // 2 to 3
	idleTime = distributor(generator);
	Engine::get_logger().log_debug("IDLE : " + std::to_string(idleTime));
}
void Trainer::TrainerStateIdle::update(GameObject*, double dt) {

	if (idleTime > 0)
		idleTime -= dt;
}
void Trainer::TrainerStateIdle::test_for_exit(GameObject* object) {
	Trainer* boss = static_cast<Trainer*>(object);

	if (idleTime < 0) {
		std::mt19937 generator((unsigned int)time(NULL));
		std::uniform_int_distribution<int> distributor(0, 1);
		shouldAttack = distributor(generator);

		if (shouldAttack == 0) {
			boss->change_state(&boss->stateAttack);
		}
		else {
			boss->change_state(&boss->stateGuard);
		}
	}
}

void Trainer::TrainerStateGuard::enter(GameObject* object) {
	Trainer* boss = static_cast<Trainer*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(TrainerAnim::GUARD));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Guard));
	std::mt19937 generator((unsigned int)time(NULL));
	std::uniform_real_distribution<double> distributor(4, 5); // 4 to 
	guardTime = distributor(generator);
	Engine::get_logger().log_debug("GUARD : " + std::to_string(guardTime));

}
void Trainer::TrainerStateGuard::update(GameObject* object, double dt) {
	Trainer* boss = static_cast<Trainer*>(object);
	boss->set_scale({ boss->is_target_left() ? -1 : 1, 1 });

	if (guardTime > 0)
		guardTime -= dt;

	boss->get_GO_components<GOAttribute>()->update_hp(0.1);
}
void Trainer::TrainerStateGuard::test_for_exit(GameObject* object) {
	Trainer* boss = static_cast<Trainer*>(object);

	if (guardTime < 0) {
		boss->change_state(&boss->stateIdle);
	}
}

void Trainer::TrainerStateAttack::enter(GameObject* object) {
	Trainer* boss = static_cast<Trainer*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(TrainerAnim::ATTACK));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::Attack));

	boss->set_scale({ boss->is_target_left() ? -1 : 1, 1 });

	attackIntervalTimer = 0.4;
}
void Trainer::TrainerStateAttack::update(GameObject* object, double dt) {
	Trainer* boss = static_cast<Trainer*>(object);
	GameObjectManager* GOM = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>();


	if (attackIntervalTimer > 0) {
		attackIntervalTimer -= dt;
		if (attackIntervalTimer < 0) {
			attackIntervalTimer = 0; // let this marker
		}
	}

	if (attackIntervalTimer != 0) return;

	double bossDamage = boss->get_GO_components<GOAttribute>()->get_ad();
	math::vec2 knockbackVector = math::vec2{ boss->get_scale().x, 1.0 } *GameObject::DEFAULT_KNOCKBACK;

	Hitbox* hitbox = new Hitbox(boss, boss->get_position(), math::rect2{ { -5, 0 }, { 100, 100 } }, bossDamage, knockbackVector);
	GOM->add_game_object(ObjType::E_Bullet, hitbox);
	attackIntervalTimer = -1;
}
void Trainer::TrainerStateAttack::test_for_exit(GameObject* object) { 
	Trainer* boss = static_cast<Trainer*>(object);

	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done()) {
		boss->change_state(&boss->stateIdle);
	}
}

