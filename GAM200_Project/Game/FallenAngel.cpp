#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Sprite.h"
#include "../Engine/Collision.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/SceneManager.h"

#include "Particles.h"
#include "Hitbox.h"
#include "GOAttribute.h"
#include "FallenAngel.h"

#include "GameMap.h"
#include "Gravity.h"
#include "../Engine/Animation.h"

FallenAngel::FallenAngel(math::vec2 pos, GameObject* target)
	: GameObject(pos), targetObject(target), phase(Phase::phase1)
{
	set_type(GOType::boss);
	set_name("Fallen Angel, The Exiled");
	add_GO_component(new Sprite("assets/Sprite/Boss/FallenAngel/FallenAngel1.spt", this));
	change_state(&stateIdle);
	add_GO_component(new GOAttribute(1000, 5));
	phase2Sprite = new Sprite("assets/Sprite/Boss/FallenAngel2/FallenAngel2.spt", this);
}

void FallenAngel::update(double dt) {
	GameObject::update(dt);
	auto objAttribute = get_GO_components<GOAttribute>();
	if (objAttribute != nullptr && objAttribute->get_hp() <= 0) {
		switch (phase)
		{
		case Phase::phase1:
			change_state(&stateCrack);
			break;
		case Phase::phase2:		
			change_state(&stateDead);
			break;
		}
	}
	Gravity* gravity = Engine::get_GE_component<Game>()->get_GS_component<Gravity>();
	if (gravity != nullptr) {
		update_velocity(math::vec2{ 0.0, -gravity->get_value() * dt });
	}
}

void FallenAngel::draw(math::TransformMatrix viewportMatrix) {
	GameObject::draw(viewportMatrix);
}


void FallenAngel::resolve_collision(GameObject*)
{
	auto attribute = get_GO_components<GOAttribute>();
	if(attribute && phase == Phase::phase1)
	{
		const bool half = attribute->get_hp() < attribute->get_max_hp() / 2.;
		if (half)
		{
			int r = rand() % 20;
			if (!r)
			{
				double heal = attribute->get_max_hp() * 0.1;
				if (attribute->get_hp() + attribute->get_max_hp() * 0.1 > attribute->get_max_hp() / 2.)
				{
					heal = attribute->get_max_hp() / 2. - attribute->get_hp();
				}
				attribute->update_hp(heal);
			}
		}
	}
}

bool FallenAngel::can_collide_with(GOType t) {
	return t == GOType::hitbox;
}

FallenAngel::Phase FallenAngel::get_phase()
{
	return phase;
}

// === States ====================================================================

void FallenAngel::FAIdle::enter(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::IDLE));
	boss->set_timer(boss->phase == Phase::phase1 ? 3 : 2);
	stateSwitch = false;
}
void FallenAngel::FAIdle::update(GameObject* object, double )
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if(!stateSwitch && boss->phase != Phase::phase1)
	{
		if (boss->targetObject->get_position().x > boss->get_position().x)
		{
			boss->set_velocity({ 0.5, 0. });
			boss->set_scale({ -1, 1 });
		}
		else
		{
			boss->set_velocity({ -0.5, 0. });
			boss->set_scale({ 1, 1 });
		}
		stateSwitch = true;
	}
}
void FallenAngel::FAIdle::test_for_exit(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	//double distanceX = std::abs(boss->get_position().x - boss->targetObject->get_position().x);
	const auto attribute = boss->get_GO_components<GOAttribute>();
 	if (boss->is_timer_done()) {
		auto r = rand();
		if (boss->phase == Phase::phase1 && attribute->get_hp() != attribute->get_max_hp())
		{
			r = r % 2;
			if(abs(boss->targetObject->get_position().x - boss->get_position().x) < 200)
			{
				r = 2;
			}
			switch (r)
			{
			case 0:
				boss->change_state(&boss->stateLaserR);
				break;
			case 1:
				boss->change_state(&boss->stateLaserL);
				break;
			case 2:
				boss->change_state(&boss->stateWingAttack);
				break;
			}
		} else if (boss->phase == Phase::phase2)
		{
			const bool half = attribute->get_hp() < attribute->get_max_hp() / 2.;
			boss->set_velocity({ 0., 0. });
			if(abs(boss->targetObject->get_position().x - boss->get_position().x) > 300)
			{
				if (half)
				{
					r = r % 4;
				} else
				{
					r = r % 8;
				}
				if(r)
				{
					boss->change_state(&boss->stateWarpIn);
				} else
				{
					boss->change_state(&boss->stateJump);
				}
			} else
			{
				boss->change_state(&boss->stateAttack);
			}
		}
	}
}

void FallenAngel::FALaserL::enter(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::LASERL));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FALaser));

	boss->set_timer(0.86);

}

void FallenAngel::FALaserL::update(GameObject* , double ) {
	//FallenAngel* boss = static_cast<FallenAngel*>(object);
}

void FallenAngel::FALaserL::test_for_exit(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if (boss->is_timer_done()) {
		for(double i = 0; i < 1000.; i += 50.)
		{
			const double k = i * 0.001;
			Hitbox* attackBox = new Hitbox(boss, math::vec2{ 1100 + i, boss->get_position().y - 70. }, math::rect2{ {0, 0}, {91, 91} }, 10, math::vec2{ 0, 0 }, 0.2, k + 0.01, false, Hitbox::Debuff::count, "assets/sprite/particles/LaserExplosionParticle.spt", Audio::Effects::FAExplosion);
			Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox);
		}
		boss->change_state(&boss->stateIdle);
	}
}

void FallenAngel::FALaserR::enter(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::LASERR));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FALaser));
	boss->set_timer(0.86);

}

void FallenAngel::FALaserR::update(GameObject*, double) {
	//FallenAngel* boss = static_cast<FallenAngel*>(object);
}

void FallenAngel::FALaserR::test_for_exit(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if (boss->is_timer_done()) {
		for (double i = 1000; i > 0; i -= 50.)
		{
			const double k = (1000-i) * 0.001;
			Hitbox* attackBox = new Hitbox(boss, math::vec2{ 1100 + i, boss->get_position().y - 70. }, math::rect2{ {0, 0}, {91, 91} }, 10, math::vec2{ 0, 0 }, 0.2, k + 0.01, false, Hitbox::Debuff::count, "assets/sprite/particles/LaserExplosionParticle.spt", Audio::Effects::FAExplosion);
			Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox);
		}
		boss->change_state(&boss->stateIdle);
	}
}

void FallenAngel::FAWingAttack::enter(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::WING));
	Hitbox* attackBox = new Hitbox(boss, boss->get_GO_components<RectCollision>()->GetWorldCoorRect().Center() + math::vec2{0, 100}, math::rect2{ {-200, -250}, {200, 50} }, 20, boss->targetObject->get_position().x < boss->get_position().x ? math::vec2{ -5, 5 } : math::vec2{ 5, 5 }, 0.2, 0.2, false, Hitbox::Debuff::count, "assets/sprite/particles/WingAttack.spt");
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox);
	boss->set_timer(0.46);
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAWing));

}

void FallenAngel::FAWingAttack::update(GameObject*, double)
{
}

void FallenAngel::FAWingAttack::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if (boss->is_timer_done())
	{
		boss->change_state(&boss->stateIdle);
	}
}

void FallenAngel::FACrack::enter(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::CRACK));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FACrack));
	Engine::get_GE_component<Input>()->set_key_work(false);
	boss->set_timer(1.5);
	boss->remove_GO_component<GOAttribute>();
	stateSwitch = false;
}

void FallenAngel::FACrack::update(GameObject* object, double)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if (boss->is_timer_done() && !stateSwitch)
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAEarthquake));
		Engine::get_GE_component<Game>()->get_game_component<Camera>()->shake({ 20, 0 }, 15, 10);
		boss->set_timer(2);
		stateSwitch = true;
	}
}

void FallenAngel::FACrack::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if (boss->is_timer_done() && stateSwitch)
	{
		boss->change_state(&boss->stateFalling);
	}
}

void FallenAngel::FAFalling::enter(GameObject*)
{
	//FallenAngel* boss = static_cast<FallenAngel*>(object);

	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->set_collidable();
	cameraFixPosition = Engine::get_GE_component<Game>()->get_game_component<Camera>()->get_target_positiion();
}

void FallenAngel::FAFalling::update(GameObject* object, double) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	Engine::get_GE_component<Game>()->get_game_component<Camera>()->set_absolute_position(boss->targetObject->get_position() - cameraFixPosition + math::vec2{ 0, 140 } + boss->targetObject->get_scale().x * math::vec2{ 100, 0 });
}

void FallenAngel::FAFalling::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if(boss->get_position().y < 200)
	{
		Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->set_collidable();
		boss->change_state(&boss->stateAdvent);
	}
}

void FallenAngel::FAAdvent::enter(GameObject* object)
{
	Engine::get_GE_component<Game>()->get_game_component<Camera>()->set_cam_extent({ {  64, 0 }, Engine::get_GE_component<Game>()->get_game_component<SceneManager>()->get_curr_scene()->get_GS_components<GameMap>()->get_stage_size() - math::vec2{64., 0.} });
	Engine::get_GE_component<Game>()->get_game_component<Camera>()->shake({ 0, 0 }, 0, 0);
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAAdvent));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAStoneBroke));
	Hitbox* attackBox = new Hitbox(boss, { boss->get_position().x, 156. }, math::rect2{ {-200, 0}, {200, 200} }, 1000, math::vec2{ 10, 7 }*boss->get_scale() * math::vec2{ -1, 0 }, 0.2, 0, false);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox);
	Engine::get_GE_component<AdventParticleEmitter>()->emit(1, { boss->get_position().x, 156. }, { 0,0 }, { 0,0 }, 0, { 1,1 }, 0);
	adventPos = boss->get_position() - math::vec2{0., 20.};
	boss->remove_GO_component<Sprite>();
	boss->remove_GO_component<Collision>();
	Hitbox* shockwaveBox1 = new Hitbox(boss, { boss->get_position().x, 156. }, math::rect2{ {-50, -10}, {50, 40} }, 20, math::vec2{ 6, 3 }, 1.15, 0, true);
	Hitbox* shockwaveBox2 = new Hitbox(boss, { boss->get_position().x, 156. }, math::rect2{ {-50, -10}, {50, 40} }, 20, math::vec2{ -6, 3 }, 1.15, 0, true);
	shockwaveBox1->set_velocity({ 3, 0 });
	shockwaveBox2->set_velocity({ -3, 0 });
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, shockwaveBox1);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, shockwaveBox2);
}

void FallenAngel::FAAdvent::update(GameObject* , double)
{
}

void FallenAngel::FAAdvent::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if (boss->is_timer_done() == true) {
		Engine::get_GE_component<Input>()->set_key_work(true);
		boss->add_GO_component(new GOAttribute(1000, 20));
		boss->add_GO_component(boss->phase2Sprite);
		boss->phase = Phase::phase2;
		boss->set_position(adventPos);
		boss->change_state(&boss->stateIdle);
	}
}

void FallenAngel::FAJump::enter(GameObject* object)
{
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAPrejump));

	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim2::JUMP));
	boss->set_timer(0.8);
	boss->set_velocity({ 0, 0 });
}

void FallenAngel::FAJump::update(GameObject*, double)
{
}

void FallenAngel::FAJump::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if(boss->is_timer_done())
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAJump));
		boss->set_velocity({ 0, 30 });
		boss->change_state(&boss->stateDescent);
	}
	
}

void FallenAngel::FADescent::enter(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	stateSwitch = false;
	soundSwitch = false;
	boss->set_timer(9999);
}

void FallenAngel::FADescent::update(GameObject* object, double)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if(!stateSwitch && boss->get_velocity().y < 0)
	{
		boss->set_position({ boss->targetObject->get_position().x, boss->get_position().y });
		stateSwitch = true;
	}

	if(stateSwitch && !soundSwitch && boss->get_position().y < 1800)
	{
		boss->set_position({ boss->targetObject->get_position().x, boss->get_position().y });
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FADescent));
		soundSwitch = true;
	}

	if (stateSwitch && boss->get_position().y < 200)
	{
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim2::DESCENT));
		boss->change_state(&boss->stateShockwave);
		boss->set_timer(0.7);
	}
}

void FallenAngel::FADescent::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if (stateSwitch && boss->is_timer_done())
	{
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim2::DESCENT));
		boss->change_state(&boss->stateShockwave);
	}
}

void FallenAngel::FAShockwave::enter(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	Hitbox* attackBox = new Hitbox(boss, boss->get_position(), math::rect2{ {-200, 0}, {200, 200} }, 1000, math::vec2{ 10, 7 }*boss->get_scale() * math::vec2{ -1, 0 }, 0.2, 0, true);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox);
	Engine::get_GE_component<AdventParticleEmitter>()->emit(1, { boss->get_position().x, 156. }, { 0,0 }, { 0,0 }, 0, { 1,1 }, 0);

	Hitbox* shockwaveBox1 = new Hitbox(boss, { boss->get_position().x, 156. }, math::rect2{ {-50, -10}, {50, 40} }, 20, math::vec2{ 6, 3 }, 0.8, 0, true);
	Hitbox* shockwaveBox2 = new Hitbox(boss, { boss->get_position().x, 156. }, math::rect2{ {-50, -10}, {50, 40} }, 20, math::vec2{ -6, 3 }, 0.8, 0, true);
	shockwaveBox1->set_velocity({ 4, 0 });
	shockwaveBox2->set_velocity({ -4, 0 });
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, shockwaveBox1);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, shockwaveBox2);;
}

void FallenAngel::FAShockwave::update(GameObject* , double)
{
}

void FallenAngel::FAShockwave::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done())
	{
		boss->change_state(&boss->stateIdle);
	}
}

void FallenAngel::FAWarpIn::enter(GameObject* object)
{
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAWarpIn));

	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<GOAttribute>()->set_df(1);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim2::WARPIN));
	const double timer = rand() % 4096 / 2048. + 1.2;
	boss->set_timer(timer);
}

void FallenAngel::FAWarpIn::update(GameObject*, double)
{
}

void FallenAngel::FAWarpIn::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if (boss->is_timer_done())
	{
		const int c = rand() % 2;
		if(!c)
		{
			boss->set_position(boss->targetObject->get_position() + math::vec2{100, 0});
			boss->set_scale({ 1, 1 });
		} else
		{
			boss->set_position(boss->targetObject->get_position() + math::vec2{ -100, 0 });
			boss->set_scale({ -1, 1 });
		}
		boss->change_state(&boss->stateWarpAttack);
	}
}

void FallenAngel::FAWarpAttack::enter(GameObject* object)
{
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAWarpOut));

	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim2::WARPOUT));
	boss->set_timer(0.8);
	stateSwitch = false;
}

void FallenAngel::FAWarpAttack::update(GameObject* object, double)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if(!stateSwitch && boss->is_timer_done())
	{
		boss->get_GO_components<GOAttribute>()->set_df(0);
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAStrongAttack));

		Hitbox* attackBox = new Hitbox(boss, boss->get_position(), math::rect2{ {-200, 0}, {0, 200} }, 35, math::vec2{ 5, 5 }*boss->get_scale() * math::vec2{ -1, 1 }, 0.2, 0.4, true, Hitbox::Debuff::count, "", Audio::Effects::Count, Audio::Effects::FAHit);
		Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox);
		stateSwitch = true;
	}
}

void FallenAngel::FAWarpAttack::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done())
	{
		boss->change_state(&boss->stateIdle);
	}
}

void FallenAngel::FAAttack::enter(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if (boss->targetObject->get_position().x > boss->get_position().x)
	{
		boss->set_scale({ -1, 1 });
	}
	else
	{
		boss->set_scale({ 1, 1 });
	}
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim2::ATTACK));
	Hitbox* attackBox1 = new Hitbox(boss, boss->get_position(), math::rect2{ {-200, 0}, {0, 200} }, 15, math::vec2{ 3, 3 } * boss->get_scale() * math::vec2{ -1, 1 }, 0.2, 0.5, true, Hitbox::Debuff::count, "", Audio::Effects::Count, Audio::Effects::FAHit);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox1);
	Hitbox* attackBox2 = new Hitbox(boss, boss->get_position(), math::rect2{ {-200, 0}, {0, 200} }, 22, math::vec2{ 3, 3 } * boss->get_scale() * math::vec2{ -1, 1 }, 0.2, 1.0, true, Hitbox::Debuff::count, "", Audio::Effects::Count, Audio::Effects::FAHit);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox2);
	Hitbox* attackBox3 = new Hitbox(boss, boss->get_position(), math::rect2{ {-200, 0}, {0, 200} }, 35, math::vec2{ 5, 5 } * boss->get_scale() * math::vec2{ -1, 1 }, 0.2, 1.7, true, Hitbox::Debuff::count, "", Audio::Effects::Count, Audio::Effects::FAHit);
	Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attackBox3);
	const int r = rand() % 5;
	switch (r)
	{
	case 0:
		attackBox1->update_position(math::vec2{ abs(boss->targetObject->get_position().x - boss->get_position().x) / 2 * boss->get_scale().x * -1, 0. });
		attackBox2->update_position(math::vec2{ abs(boss->targetObject->get_position().x - boss->get_position().x) / 2 * boss->get_scale().x * -1, 0. });
		attackBox3->update_position(math::vec2{ abs(boss->targetObject->get_position().x - boss->get_position().x) / 2 * boss->get_scale().x * -1, 0. });
		boss->set_timer(0.4);
		break;
	case 1:
		attackBox2->update_position(math::vec2{ abs(boss->targetObject->get_position().x - boss->get_position().x) / 2 * boss->get_scale().x * -1, 0. });
		attackBox3->update_position(math::vec2{ abs(boss->targetObject->get_position().x - boss->get_position().x) / 2 * boss->get_scale().x * -1, 0. });
		boss->set_timer(0.9);
		break;
	default:
		boss->set_timer(0);
		break;
	}
	stateSwitch = false;
}

void FallenAngel::FAAttack::update(GameObject* object, double)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if(!stateSwitch && boss->is_timer_done())
	{
		boss->update_position(math::vec2{ abs(boss->targetObject->get_position().x - boss->get_position().x) / 2 * boss->get_scale().x * -1, 0. } /** boss->get_scale() * math::vec2{ -1 , 0}*/);
		stateSwitch = true;
	}
	const int index = boss->get_GO_components<Sprite>()->get_anim()->get_anim_index();

	if (index == 3 || index == 8)
	{
		if(!soundSwitch)
		{
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAAttack));
			soundSwitch = true;
		}
	}
	else if (index == 15)
	{
		if (!soundSwitch)
		{
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FAStrongAttack));
			soundSwitch = true;
		}
	}
	else
	{
		soundSwitch = false;
	}
}

void FallenAngel::FAAttack::test_for_exit(GameObject* object)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done())
	{
		boss->change_state(&boss->stateIdle);
	}
}

void FallenAngel::FADead::enter(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim2::DEAD));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FADead));
	boss->remove_GO_component<GOAttribute>();
	boss->set_velocity({ 0, 0 });
	boss->set_timer(1.3);
	stateSwitch = false;
}

void FallenAngel::FADead::update(GameObject* object, double)
{
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if(!stateSwitch && boss->is_timer_done())
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::FADeadEffect));
		stateSwitch = true;
	}
}

void FallenAngel::FADead::test_for_exit(GameObject* object) {
	FallenAngel* boss = static_cast<FallenAngel*>(object);

	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() == true) {
		boss->set_is_dead(true);
		boss->remove_GO_component<RectCollision>();
	}
}

