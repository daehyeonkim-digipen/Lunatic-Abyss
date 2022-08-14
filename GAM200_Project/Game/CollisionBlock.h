#pragma once
#include "../Engine/Input.h"
#include "../Engine/GameObject.h"

class CollisionBlock : public GameObject
{
public:
	CollisionBlock(math::vec2 pos, math::vec2 size);
	~CollisionBlock();

	void resolve_collision(GameObject*) override;
	void draw(math::TransformMatrix viewportMatrix) override;
	bool can_collide_with(GOType t) override;
};