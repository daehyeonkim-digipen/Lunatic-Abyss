#pragma once
#include <map>

#include "Component.h"
#include "Math.h"
class GameObject;

class Collision : public Component {
public:
    virtual bool does_collide(GameObject* gameObject) = 0;
    virtual bool does_collide(math::vec2 point) = 0;
	virtual void draw(math::TransformMatrix displaymt) = 0;
};

class RectCollision : public Collision
{
	math::rect2 rect;
public:
	enum class RectCollideType : int
	{
		 LT, RT, LM, RM, LB, RB, All,
	};
	RectCollision(math::rect2 rect, GameObject* owner);
	void draw(math::TransformMatrix displaymt);
	math::rect2 GetWorldCoorRect();
	bool does_collide(GameObject* obj) override;
	bool does_collide(math::vec2 point) override;
	void will_collide(GameObject* obj, bool collisions[]);
private:
	GameObject* owner;
};