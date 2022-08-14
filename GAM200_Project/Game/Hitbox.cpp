#include "../Engine/Collision.h"
#include "../Engine/Engine.h"
#include "../Engine/Audio.h"
#include "../Engine/Game.h"
#include "../Engine/Sprite.h"
#include "Player.h"
#include "Hitbox.h"
#include "Particles.h"
#include <typeinfo>
#include "GOAttribute.h"
#include "../Engine/Animation.h"

Hitbox::Hitbox(GameObject* owner_, math::vec2 position, math::rect2 rect, double damage_, math::vec2 knockback_, double lifeTime, double wait, bool defendable, Debuff debuff, std::string hitboxSprite, Audio::Effects effectsound, Audio::Effects hitSound)
	: GameObject(position), rect(rect), owner(owner_), damage(damage_), knockback(knockback_), lifetime(lifeTime), wait(wait), defendable(defendable), work(false), debuff(debuff), effectSound(effectsound), hitSound(hitSound)
{
	if (!hitboxSprite.empty()) {
		add_GO_component(new Sprite(hitboxSprite, this));
	}
	set_type(GOType::hitbox);
	set_scale(owner->get_scale());
	soundSwitch = false;
}

void Hitbox::update(double dt)
{
	if(work)
	{
		if (effectSound != Audio::Effects::Count && !soundSwitch)
		{
			Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(effectSound));
			soundSwitch = true;
		}
		if (get_GO_components<Sprite>() != nullptr) {
			get_GO_components<Sprite>()->update(dt);
			if (get_GO_components<Sprite>()->is_animation_done()) {
				set_is_dead(true);
			} else if (get_GO_components<Sprite>()->get_anim()->get_type() == Animation::Command::Loop)
			{
				if (lifetime <= 0) {
					set_is_dead(true);
				}
			}
		}
		else
		{
			if (lifetime <= 0) {
				set_is_dead(true);
			}
		}
		if (lifetime > 0) {
			lifetime -= dt;
		}

	} else
	{
		if (wait > 0)
		{
			wait -= dt;
		}
		else
		{
			add_GO_component(new RectCollision(rect, this));
			wait = 0;
			work = true;
			if (get_GO_components<Sprite>())
				get_GO_components<Sprite>()->play_animation(0);
		}
	}
}

void Hitbox::draw(math::TransformMatrix viewportMatrix)
{
	if (work)
	{
		GameObject::draw(viewportMatrix);
	}
}

void Hitbox::resolve_collision(GameObject* object) {
	auto objType = object->get_type();
	bool doesObjHostile = objType == GOType::enemy || objType == GOType::boss;

	if ((owner->get_type() != GOType::player && objType == GOType::player) || (owner->get_type() == GOType::player && doesObjHostile)) {
		if (does_hitted(object) == false) {
			const auto objAttribute = object->get_GO_components<GOAttribute>();
			const auto ownerAttribute = owner->get_GO_components<GOAttribute>();

			if (owner->get_type() == GOType::player && doesObjHostile) {
				Engine::get_GE_component<Game>()->set_hitstop();
			}


			if (objAttribute != nullptr) {
				if (!defendable)
				{
					auto player = dynamic_cast<Player*>(object);
					if(player->get_ps().FTH1)
					{
						objAttribute->set_df(objAttribute->get_df() * 0.8);

					} else
					{
						objAttribute->set_df(0);
					}
				}
				objAttribute->update_hp(-damage);
                if(ownerAttribute && ownerAttribute->get_max_gage())
                {
                    ownerAttribute->update_gage(10);
                }
				if(debuff != Debuff::count && objAttribute->get_df() < 1)
				{
					auto player = dynamic_cast<Player*>(object);
					if(player)
					{
						switch(debuff)
						{
						case Debuff::confusion:
							player->get_ps().confusion = true;
							player->get_ps().confusionTimer = 5.0;
							break;
						case Debuff::freeze:
							player->get_ps().freeze = true;
							player->get_ps().freezeTimer = 5.0;
							break;
						}

					}
				}
			}
			if (hitSound != Audio::Effects::Count && !objAttribute->get_df()) Engine::get_GE_component<Audio>()->play_sound(static_cast<int>(hitSound));

			object->resolve_collision(this);
			hittedTargets.push_back(object);
			object->update_velocity(knockback);

		}
	}

}

bool Hitbox::can_collide_with(GOType _type) {
	switch (_type) {
	case GOType::player: [[fallthrough]];
	case GOType::enemy: [[fallthrough]];
	case GOType::boss:
		return true;
	default:
		return false;
	}
}

bool Hitbox::does_hitted(GameObject* target) {
	return std::find(hittedTargets.begin(), hittedTargets.end(), target) != hittedTargets.end();
}

void Hitbox::no_knockback()
{
	knockback /= 10.;
}

void Hitbox::superArmor()
{
	knockback *= 0.;
}

bool Hitbox::is_defendable()
{
	return defendable;
}

bool Hitbox::is_working()
{
	return work;
}

