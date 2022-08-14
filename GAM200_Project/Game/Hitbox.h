#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Audio.h"
class Hitbox : public GameObject {
	friend class Player;
	friend class Enemy;
public://Basic Methods
	enum class Debuff
	{
		freeze,
		confusion,
		count
	};
	Hitbox(GameObject* owner, math::vec2 position, math::rect2 rect, double damage, math::vec2 knockback, double lifeTime = 0.1, double wait = 0, bool defendable = true, Debuff shock = Debuff::count, std::string hitboxSptire = "", Audio::Effects effectSound = Audio::Effects::Count, Audio::Effects hitSound = Audio::Effects::Count);
	void update(double dt) override;
	void draw(math::TransformMatrix displayMt) override;
	void resolve_collision(GameObject* object) override;
	bool can_collide_with(GOType t) override;

public://Hitbox Status
	bool does_hitted(GameObject* target);
	GameObject* get_owner() { return owner; }
	void no_knockback();
	void superArmor();
	bool is_defendable();
	bool is_working();
private:
	math::rect2 rect;
	Audio::Effects hitSound;
	Audio::Effects effectSound;
	bool defendable;
	bool work;
	bool soundSwitch;
	Debuff debuff;
	double wait;
	double lifetime;
	double damage;
	math::vec2 knockback;
	GameObject* owner;
	std::vector<GameObject*> hittedTargets;
};

