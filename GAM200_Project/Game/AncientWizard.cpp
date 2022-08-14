#include "../Engine/Engine.h"
#include "../Engine/Game.h"
#include "../Engine/Sprite.h"
#include "../Engine/Collision.h"
#include "../Engine/Camera.h"
#include "../Engine/GameObjectManager.h"
#include "Player.h"
#include "Hitbox.h"
#include "GOAttribute.h"
#include "AncientWizard.h"

#include "GameMap.h"
#include "Gravity.h"
#include "Particles.h"
#include "../Engine/Animation.h"
#include "../Engine/TextureManager.h"

AncientWizard::AncientWizard(math::vec2 pos, Player* target)
	: GameObject(pos), targetObject(target)
{
	set_type(GOType::boss);
	set_name("The Nameless One, Master of The Abyss");
	add_GO_component(new Sprite("assets/Sprite/Boss/AncientWizard/AncientWizard.spt", this));
	light = new GameObject({ 0, 0 });
	light->add_GO_component(Engine::get_GE_component<TextureManager>()->load("assets/UI/transparency.png"));
	light->get_GO_components<Texture>()->set_shader("assets/Shaders/e_default.vert", "assets/Shaders/e_blindlight.frag");
	light->set_scale({30, 30});
	blackhole = new GameObject({ 0, 0 });
	blackhole->add_GO_component(Engine::get_GE_component<TextureManager>()->load("assets/UI/transparency2.png"));
	blackhole->get_GO_components<Texture>()->set_shader("assets/Shaders/e_default.vert", "assets/Shaders/e_blackhole.frag");
	blackhole->set_scale({ 5, 5 });
	blackhole->set_position(Engine::get_GE_component<Game>()->get_GS_component<GameMap>()->get_stage_size() / 2. - blackhole->get_GO_components<Texture>()->get_size() * blackhole->get_scale().x / 2);

	change_state(&stateIdle);
	add_GO_component(new GOAttribute(1000, 5));
}

void AncientWizard::update(double dt)
{
	set_velocity({ 0, 0 });
	GameObject::update(dt);
	auto objAttribute = get_GO_components<GOAttribute>();
	if (objAttribute != nullptr && objAttribute->get_hp() <= 0)
		change_state(&stateDead);
}

void AncientWizard::draw(math::TransformMatrix viewportMatrix)
{
	GameObject::draw(viewportMatrix);
	if(blindOn)
	{
		light->draw(viewportMatrix);
	}
	if (blackholeOn)
	{
		blackhole->draw(viewportMatrix);
	}
}

void AncientWizard::resolve_collision(GameObject* gameObject)
{
	GameObject::resolve_collision(gameObject);
}

bool AncientWizard::can_collide_with(GOType t)
{
	return t == GOType::hitbox;
}

bool AncientWizard::is_teleported()
{
	return teleport;
}

void AncientWizard::set_teleported()
{
	teleport = !teleport;
}

bool AncientWizard::is_ult()
{
	return ult;
}


void AncientWizard::AWIdle::enter(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::IDLE));
	boss->set_timer(2.0);
}

void AncientWizard::AWIdle::update(GameObject* object, double dt)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (abs(boss->targetObject->get_position().x - boss->get_position().x) < 200)
	{
		tpTimer -= dt;
	}

	if (boss->targetObject->get_position().x > boss->get_position().x)
	{
		boss->set_scale({ -1, 1 });
	}
	else
	{
		boss->set_scale({ 1, 1 });
	}

}

void AncientWizard::AWIdle::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	//double distanceX = std::abs(boss->get_position().x - boss->targetObject->get_position().x);
	if(tpTimer <= 0)
	{
		auto r = rand() % 2;
		if(r)
		{
			tpTimer = 1;
		} else
		{
			tpTimer = 3;
			boss->change_state(&boss->stateTeleport);
		}
	}
	else if (boss->is_timer_done()) {
		auto r = rand() % 9;
		switch (r)
		{
		case 0:
		case 1:
			boss->change_state(&boss->stateBlind);
			break;
		case 2:
		case 3:
			boss->change_state(&boss->stateLaser);
			break;
		case 4:
			boss->ult = true;
			boss->change_state(&boss->stateTeleport);
			break;
		default:
			if (glm::distance(glm::vec2(boss->targetObject->get_position().x, boss->targetObject->get_position().y), glm::vec2(boss->get_position().x, boss->get_position().y)) < 300)
			{
				boss->change_state(&boss->stateAttack);
			}
			else
			{
				boss->change_state(&boss->stateIce);
			}
			break;

		}
	}
}

void AncientWizard::AWBlind::enter(GameObject* object)
{

	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (!boss->blindOn)
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWBlindLight));
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::MAGIC));
	}
	else
	{
		boss->set_timer(3);
	}
}

void AncientWizard::AWBlind::update(GameObject* object, double)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);

	if(boss->blindOn && boss->targetObject && boss->targetObject->get_GO_components<RectCollision>())
	{
		math::vec2 start = boss->get_position() + math::vec2{ -boss->light->get_GO_components<Texture>()->get_size().x * boss->light->get_scale().x / 2., 100. };
		math::vec2 btop = boss->targetObject->get_GO_components<RectCollision>()->GetWorldCoorRect().Center() - start;
		if(std::abs(btop.y) < 300 && (btop.x > 0 && boss->targetObject->get_scale().x == -1 || btop.x < 0 && boss->targetObject->get_scale().x == 1))
		{
			boss->targetObject->get_ps().blind = true;
			boss->targetObject->get_ps().blindTimer = 5;

		}
	}
}

void AncientWizard::AWBlind::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() && !boss->blindOn)
	{
		math::vec2 start = boss->get_position() + math::vec2{-boss->light->get_GO_components<Texture>()->get_size().x * boss->light->get_scale().x / 2., 100. };
		boss->light->set_position(start);
		boss->blindOn = true;
		boss->change_state(&boss->stateBlind);

	} else if(boss->is_timer_done() && boss->blindOn)
	{
		boss->blindOn = false;
		boss->change_state(&boss->stateIdle);
	}
}

void AncientWizard::AWIce::enter(GameObject* object)
{
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWIce));

	AncientWizard* boss = static_cast<AncientWizard*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::ATTACK));
	stateSwitch = false;
}

void AncientWizard::AWIce::update(GameObject* object, double)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);

	if(boss->get_GO_components<Sprite>()->get_anim()->get_anim_index() == 2 && stateSwitch == false)
	{
		math::vec2 start = boss->get_position() + math::vec2{ boss->get_scale().x * 30., 75. };
		math::vec2 btop = boss->targetObject->get_position() - start;

		Hitbox* Icebolt = new Hitbox(boss, start, math::rect2{ {-20, 20}, {0, 40} }, 20, math::vec2{ 1 * btop.Normalize().x, 2. }, 10, 0, true, Hitbox::Debuff::freeze,"assets/sprite/particles/icebolt.spt", Audio::Effects::Count, Audio::Effects::AWIceBoltHit);
		double angle = atan2(btop.y, btop.x);
		Icebolt->set_scale({ -1, 1 });
		Icebolt->set_rotation(angle);
		Icebolt->set_velocity(math::RotateMatrix(Icebolt->get_rotation()) * math::vec2{ 2.5, 0. });
		Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, Icebolt);
		stateSwitch = true;
	}
}

void AncientWizard::AWIce::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done())
	{
		boss->change_state(&boss->stateIdle);
	}
}

void AncientWizard::AWLaser::enter(GameObject* object)
{

	AncientWizard* boss = static_cast<AncientWizard*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::MAGIC));
	stateSwitch = false;
}

void AncientWizard::AWLaser::update(GameObject* object, double)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);

	if(!stateSwitch && boss->targetObject->get_velocity().y == 0)
	{
		pos = boss->targetObject->get_position();
		Engine::get_GE_component<TargetingParticleEmitter>()->emit(1, pos, { 0, 0 }, { 0, 0 }, 0, {0.5, 0.5});
		stateSwitch = true;
	}
}

void AncientWizard::AWLaser::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done())
	{
		Hitbox* Laser = new Hitbox(boss, pos, math::rect2{ {-30, 0}, {30, 600} }, 30, math::vec2{ 0., 5. }, 1.2, 0, false, Hitbox::Debuff::confusion, "assets/sprite/particles/laser.spt", Audio::Effects::AWLaser);
		Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, Laser);
		boss->change_state(&boss->stateIdle);
	}
}

void AncientWizard::AWAttack::enter(GameObject* object)
{
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWSummon));
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::ATTACK2));
	Engine::get_GE_component<SpawnSwordParticleEmitter>()->emit(1, boss->get_position() - math::vec2{boss->get_scale().x  * 350., 150.}, { 0, 0 }, { 0, 0 }, 0, boss->get_scale());
	boss->set_timer(0.6);
	stateSwitch = false;
	soundSwitch = false;
}

void AncientWizard::AWAttack::update(GameObject* object, double)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (boss->is_timer_done() && !soundSwitch)
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWAttack));
		boss->set_timer(0.6);
		soundSwitch = true;

	}
	else if(boss->is_timer_done() && !stateSwitch)
	{
		Hitbox* attack = new Hitbox(boss, boss->get_position(), math::rect2{ {-50, 0}, {-280, 300} }, 30, math::vec2{ 0., 5. }, 0.3, 0, true);
		Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>()->add_game_object(ObjType::E_Bullet, attack);
		stateSwitch = true;
	}
}

void AncientWizard::AWAttack::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);

	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() == true) {
		boss->change_state(&boss->stateIdle);
	}
}

void AncientWizard::AWTeleport::enter(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
 	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::TELIN));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWTeleport));

}

void AncientWizard::AWTeleport::update(GameObject* object, double)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if(boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() && !stateSwitch)
	{
		boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::TELOUT));
		boss->teleport = true;
		stateSwitch = true;
	}
}

void AncientWizard::AWTeleport::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() && stateSwitch)
	{
		stateSwitch = false;
		if(boss->ult)
		{
			boss->change_state(&boss->stateBlackHole);
		} else
		{
			boss->change_state(&boss->stateIdle);
		}
	}
}

void AncientWizard::AWBlackHole::enter(GameObject* object)
{
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWBlackHole));
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWSay));

	AncientWizard* boss = static_cast<AncientWizard*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::BLACKHOLE));
	boss->set_timer(1);
	stateSwitch = false;
}

void AncientWizard::AWBlackHole::update(GameObject* object, double)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if(boss->is_timer_done() && !stateSwitch)
	{
		Engine::get_GE_component<Input>()->set_key_work(false);
		boss->blackholeOn = true;
		stateSwitch = true;
		boss->set_timer(4);
	}

	if(boss->blackholeOn)
	{
		if (!boss->targetObject->get_GO_components<RectCollision>()) return;
		math::vec2 toblackhole = Engine::get_GE_component<Game>()->get_GS_component<GameMap>()->get_stage_size() / 2. - boss->targetObject->get_GO_components<RectCollision>()->GetWorldCoorRect().Center();
		if(toblackhole.Length() > 30)
		{
			boss->targetObject->set_velocity(toblackhole.Normalize() * 5);

		} else
		{
			if (boss->get_timer() > 0.5)
			{
				boss->set_timer(0.5);
			}
		}
	}
}

void AncientWizard::AWBlackHole::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if (boss->is_timer_done() && stateSwitch)
	{
		math::vec2 toblackhole = Engine::get_GE_component<Game>()->get_GS_component<GameMap>()->get_stage_size() / 2. - boss->targetObject->get_GO_components<RectCollision>()->GetWorldCoorRect().Center();
		if (toblackhole.Length() < 50)
		{
			auto targetAttri = boss->targetObject->get_GO_components<GOAttribute>();
			targetAttri->update_hp(-targetAttri->get_hp());
			boss->targetObject->remove_GO_component<Sprite>();
		}
		boss->blackholeOn = false;
		boss->ult = false;
		boss->change_state(&boss->stateIdle);
		Engine::get_GE_component<Input>()->set_key_work(true);
	}
}

void AncientWizard::AWDead::enter(GameObject* object)
{
	Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWDie));

	AncientWizard* boss = static_cast<AncientWizard*>(object);
	boss->get_GO_components<Sprite>()->play_animation(static_cast<int>(Anim::DEAD));
	boss->remove_GO_component<GOAttribute>();
	boss->set_timer(1);
}

void AncientWizard::AWDead::update(GameObject* object, double)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);
	if(!stateSwitch && boss->is_timer_done())
	{
		Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(Audio::Effects::AWDieEffect));
		stateSwitch = true;
	}
}

void AncientWizard::AWDead::test_for_exit(GameObject* object)
{
	AncientWizard* boss = static_cast<AncientWizard*>(object);

	if (boss->get_GO_components<Sprite>() && boss->get_GO_components<Sprite>()->is_animation_done() == true) {
		boss->set_is_dead(true);
		boss->remove_GO_component<RectCollision>();
	}
}
