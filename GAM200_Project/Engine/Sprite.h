#pragma once
#include <string>
#include <vector>
#include <glm/vec4.hpp>

#include "Math.h"
#include "Component.h"


class Texture;
class Animation;
class GameObject;

class Sprite : public Component {
public:
	Sprite(const std::string& spriteInfoFile, GameObject* object);
	~Sprite();
	void load(const std::string& spriteInfoFile, GameObject* object);
	void update(double dt) override;
	void draw(math::TransformMatrix displayMatrix);
	void play_animation(int anim);
	int get_current_anim();
	Animation* get_anim();
	bool is_animation_done();
	void set_frame_speed(int index, double speed);
	math::vec2 get_hot_spot(int index);
	math::vec2 get_frame_size() const;
	void set_damaged(bool input);
	void set_tint(double r, double g, double b, double a = 1.0);
private:
	math::vec2 get_frame_texel(int frameNum) const;
	bool damaged = false;
	Texture* texturePtr;
	int currAnim;
	math::vec2 frameSize;
	std::vector<math::vec2> frameTexel;
	std::vector<Animation*> animations;
	std::vector<math::vec2> hotSpotList;
	glm::vec4 uColor{ 1, 1, 1, 1 };

	std::string pathhh;
};
