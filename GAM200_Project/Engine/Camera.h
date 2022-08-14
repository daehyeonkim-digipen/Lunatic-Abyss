#pragma once
#include "Math.h"
#include "Component.h"

namespace math {
	class TransformMatrix;
}

class GameObject;
class Camera : public Component {
public:
	void set_movable_range(math::rect2 range);

	void update(double dt);

	// move camera to given coordinate with easing
	void set_cam_position(math::vec2 newPosition);

	// set the camera without easing
	void set_absolute_position(math::vec2 newPosition);

	const math::vec2& get_position() const;
	const math::rect2& get_extent() const;
	void set_cam_extent(math::rect2 extent);
	void cam_update_for(GameObject* object);

	void shake(math::vec2 intensity, double duration, double frequency);

	math::TransformMatrix get_matrix();
	math::vec2 get_target_positiion();
private:
	double shake_noise(double value);

	double shakeDurationTime = 0;
	double shakeFrequency = 0;
	math::vec2 shakeIntensity;

	math::vec2 followObjPos;

	math::rect2 extent;
	math::vec2 destination;
	math::vec2 position;
	math::rect2 movableRange;
};
