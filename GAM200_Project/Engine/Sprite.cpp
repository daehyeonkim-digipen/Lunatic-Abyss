#include "Engine.h"			//Engine::get_logger
#include "Sprite.h"
#include "Animation.h"
#include "Collision.h"
#include "Texture.h"
#include "TextureManager.h"
#include "GameObject.h"

Sprite::Sprite(const std::string& spriteInfoFile, GameObject* object) {
	load(spriteInfoFile, object);
	pathhh = spriteInfoFile;
}

Sprite::~Sprite() {
	for (Animation* anim : animations) {
		delete anim;
	}
	animations.clear();
}

void Sprite::load(const std::string& spriteInfoFile, GameObject* object) {
	animations.clear();
	hotSpotList.clear();
	frameTexel.clear();
	std::ifstream inFile(spriteInfoFile);

	if (inFile.is_open() == false) {
		throw std::runtime_error("Failed to load " + spriteInfoFile);
	}

	std::string text, path;
	inFile >> path; // path before files
	inFile >> text; // texture file name
	texturePtr = Engine::get_GE_component<TextureManager>()->load(path + text);
	frameSize = texturePtr->get_size();

	inFile >> text; // first key
	while (inFile.eof() == false) {
		if (text == "FrameSize") {
			inFile >> frameSize.x;
			inFile >> frameSize.y;
		}
		else if (text == "FrameGrid") {
			int cols, rows;
			const unsigned int MARKER_COLOR = 0xFFFF00FF;  // pure magenta
			inFile >> cols;
			inFile >> rows;
			for (int row = 0; row < rows; row++) {
				for (int col = 0; col < cols; col++) {
					int offsetX = static_cast<int>(frameSize.x) * col;
					int offsetY = static_cast<int>(frameSize.y) * row;

					unsigned int framePixelColor = texturePtr->get_pixel(offsetX, offsetY); // debugging
					if (framePixelColor != MARKER_COLOR)
						frameTexel.push_back({ (double)offsetX, (double)offsetY });
				}
			}
		}
		else if (text == "Frame") {
			int frameLocationX, frameLocationY;
			inFile >> frameLocationX;
			inFile >> frameLocationY;
			frameTexel.push_back({ frameLocationX, frameLocationY });
		}
		else if (text == "HotSpot") {
			double hotSpotX, hotSpotY;
			inFile >> hotSpotX;
			inFile >> hotSpotY;
			hotSpotList.push_back({ hotSpotX, frameSize.y - hotSpotY });
		}
		else if (text == "Anim") {
			inFile >> text; // only file name
			animations.push_back(new Animation(path + text));
		}
		else if (text == "CollisionRect") {
			math::rect2 rect;
			inFile >> rect.point1.x >> rect.point1.y >> rect.point2.x >> rect.point2.y;
			if (object == nullptr) {
				Engine::get_logger().log_error("Trying to add collision to a nullobject");
			}
			else {
				object->add_GO_component(new RectCollision(rect, object));
			}
		}
		else {
			Engine::get_logger().log_error("Unknown spt command " + text);
		}
		inFile >> text;
	}
	if (frameTexel.empty() == true) {
		frameTexel.push_back({ 0,0 });
	}
	if (animations.empty() == true) {
		 animations.push_back(new Animation());
		 play_animation(0);
	}
}

math::vec2 Sprite::get_hot_spot(int index) {
	if (hotSpotList.empty() || index >= hotSpotList.size()) {
		//Engine::get_logger().log_error("Sprite is missing hotspot index: " + std::to_string(index));
		return { 0,0 };
	}
	return hotSpotList[index];
}

void Sprite::play_animation(int anim) {
	if (anim < 0 || anim >= animations.size()) {
		Engine::get_logger().log_error("Animation " + std::to_string(anim) + " not found : " + pathhh);
		currAnim = 0;
	}
	else {
		currAnim = anim;
		animations[currAnim]->reset_animation();
	}
}

int Sprite::get_current_anim() {
	return currAnim;
}

Animation* Sprite::get_anim()
{
	return animations[currAnim];
}

bool Sprite::is_animation_done() {
	return animations[currAnim]->is_animation_done();
}

void Sprite::set_frame_speed(int index, double speed)
{
	animations[index]->set_frame_speed(speed);
};

void Sprite::update(double dt) {
	animations[currAnim]->update(dt);
}

math::vec2 Sprite::get_frame_size() const {
	return frameSize;
}

void Sprite::set_damaged(bool input)
{
	damaged = input;
}

void Sprite::set_tint(double r, double g, double b, double a) {
	uColor = { (float)r, (float)g, (float)b, (float)a };
}

math::vec2 Sprite::get_frame_texel(int frameNum) const {
	if (frameNum < 0 || frameNum > frameTexel.size()) {
		Engine::get_logger().log_error("Bad frame request: " + std::to_string(frameNum));
		return { 0, 0 };
	}
	else {
		return frameTexel[frameNum];
	}
}

void Sprite::draw(math::TransformMatrix displayMatrix) {
	if(damaged)
	{
		texturePtr->set_damaged(true);
	} else
	{
		texturePtr->set_damaged(false);
	}
	texturePtr->set_tint(uColor.r, uColor.g, uColor.b, uColor.a );
	texturePtr->draw(displayMatrix * math::TranslateMatrix(-get_hot_spot(0)), get_frame_texel(animations[currAnim]->get_display_frame()), get_frame_size());

}