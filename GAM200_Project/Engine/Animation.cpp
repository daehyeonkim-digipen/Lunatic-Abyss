#include <fstream>	// file input stuff
#include "Engine.h"	// Engine::get_logger
#include "Animation.h"

Animation::Animation() : Animation("./assets/Gameobject/none.anm") { }

Animation::Animation(const std::string& fileName) : animSequenceIndex(0) {
	std::ifstream inFile(fileName);
	std::string label;

	if (inFile.is_open() == false) {
		throw std::runtime_error("Failed to load " + fileName);
	}

	while (inFile.eof() == false) {
		inFile >> label;
		if (label == "PlayFrame") {
			int frame;
			float targetTime;
			inFile >> frame;
			inFile >> targetTime;

			animation.push_back(new PlayFrame(frame, targetTime));
		}
		else if (label == "Loop") {
			int loopToIndex;
			inFile >> loopToIndex;
			animation.push_back(new Loop(loopToIndex));
		}
		else if (label == "End") {
			animation.push_back(new End());
		}
		else {
			Engine::get_logger().log_error("Unknown command " + label + " in anm file " + fileName);
		}
	}
	reset_animation();
}

Animation::~Animation() {
	for (CommandData* command : animation) {
		delete command;
	}
	animation.clear();
}

void Animation::update(double dt) {
	currPlayFrameData->update(dt);
	if (currPlayFrameData->is_frame_done() == true) {
		currPlayFrameData->reset_time();
		++animSequenceIndex;
		if (animation[animSequenceIndex]->get_type() == Command::PlayFrame) {
			currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
		}
		else if (animation[animSequenceIndex]->get_type() == Command::Loop) {
			Loop* loopData = static_cast<Loop*>(animation[animSequenceIndex]);
			animSequenceIndex = loopData->get_loop_to_index();
			if (animation[animSequenceIndex]->get_type() == Command::PlayFrame) {
				currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
			}
			else {
				Engine::get_logger().log_error("Loop does not go to PlayFrame");
				reset_animation();
			}
		}
		else if (animation[animSequenceIndex]->get_type() == Command::End) {
			isAnimationDone = true;
			animSequenceIndex = 0;
			return;
		}
	}
}

int Animation::get_display_frame() {
	return currPlayFrameData->get_frame_num();
}

void Animation::reset_animation() {
	animSequenceIndex = 0;
	currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
	isAnimationDone = false;
}

bool Animation::is_animation_done() {
	return isAnimationDone;
}

void Animation::set_frame_speed(double f)
{
	for (auto k : animation)
	{
		PlayFrame* k2 = dynamic_cast<PlayFrame*>(k);
		if (k2 != nullptr)
		{
			k2->set_factor(f);
		}
	}
}

void Animation::set_anim_index(int input)
{
	animSequenceIndex = input;
}

int Animation::get_anim_index()
{
	return animSequenceIndex;
}

Animation::Command Animation::get_type()
{
	return animation[animSequenceIndex]->get_type();
}


Animation::PlayFrame::PlayFrame(int frame, double targetTime) : frame(frame), targetTime(targetTime), timer(0) {}
void Animation::PlayFrame::update(double dt)
{
	timer += dt;
}
bool Animation::PlayFrame::is_frame_done() { return timer >= (targetTime / factor); }
void Animation::PlayFrame::reset_time() { timer = 0; }
int Animation::PlayFrame::get_frame_num() { return frame; }
void Animation::PlayFrame::set_factor(double f) { 
	factor = f;
}

Animation::Loop::Loop(int loopToIndex) : loopToIndex(loopToIndex) {}
int Animation::Loop::get_loop_to_index() { return loopToIndex; }

