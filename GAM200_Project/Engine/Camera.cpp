#include "Engine.h"
#include "Graphic.h"
#include "TextureManager.h"
#include "showCollision.h"
#include "Camera.h"
#include "GameObject.h"

void Camera::set_movable_range(math::rect2 range) {
	movableRange = range;
}

void Camera::update(double dt) {
	if (dt != 0 && shakeDurationTime >= 0) {
		destination.x += shakeIntensity.x * shake_noise(shakeDurationTime);
		destination.y += shakeIntensity.y * shake_noise(shakeDurationTime + 0.6);
		shakeDurationTime -= dt;
	}

	if (position.Distance(destination) > 0.1)
		position += (destination - position) * dt * 10;
	else
		position = destination;

}

void Camera::set_cam_position(math::vec2 newPosition)
{
	destination = newPosition;
}

void Camera::set_absolute_position(math::vec2 newPosition) {
	position = newPosition;
	destination = newPosition;
}

const math::vec2& Camera::get_position() const {
	return position;
}

const math::rect2& Camera::get_extent() const {
	return extent;
}

void Camera::set_cam_extent(math::rect2 newCamExtent)
{
	newCamExtent.point2 -= Engine::get_GE_component<IGL::Graphic>()->get_viewport_size();
	extent = newCamExtent;
}

void Camera::cam_update_for(GameObject* object) {
	//if (shakeDurationTime > 0) return;

	followObjPos = object->get_position() + math::vec2{ 0, 140 } + object->get_scale().x * math::vec2{ 100, 0 };

	if (followObjPos.x > destination.x + movableRange.Right()) {
		destination.x = followObjPos.x - movableRange.Right();
	}
	if (followObjPos.x - destination.x < movableRange.Left()) {
		destination.x = followObjPos.x - movableRange.Left();
	}
	if (followObjPos.y > destination.y + movableRange.Top()) {
		destination.y = followObjPos.y - movableRange.Top();
	}
	if (followObjPos.y - destination.y < movableRange.Bottom()) {
		destination.y = followObjPos.y - movableRange.Bottom();
	}

	if (destination.x < extent.Left()) {
		destination.x = extent.Left();
	}
	if (destination.x > extent.Right()) {
		destination.x = extent.Right();
	}
	if (destination.y < extent.Bottom()) {
		destination.y = extent.Bottom();
	}
	if (destination.y > extent.Top()) {
		destination.y = extent.Top();
	}

}

void Camera::shake(math::vec2 intensity, double duration, double frequency) {
	shakeIntensity = intensity;
	shakeDurationTime = duration;
	shakeFrequency = frequency;
}


math::TransformMatrix Camera::get_matrix() {
	float viewportRatio = IGL::Graphic::VIEWPORT_RATIO;
	return math::ScaleMatrix{ math::vec2{ viewportRatio, viewportRatio } } *math::TranslateMatrix(-position);
}

math::vec2 Camera::get_target_positiion() {
	return followObjPos - position;
}

double Camera::shake_noise(double value) {
	return 0.1 * sin(12.4 * shakeFrequency * value) + 0.2 * cos(20 * shakeFrequency * value);
}
;
