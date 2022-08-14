#pragma once

#include <vector>	// std::vector

class Animation {

public:
	enum class Command {
		PlayFrame,
		Loop,
		End,
	};
	Animation();
	~Animation();
	Animation(const std::string& fileName);
	void update(double dt);
	int get_display_frame();
	void reset_animation();
	bool is_animation_done();
	void set_frame_speed(double);
	void set_anim_index(int input);
	int get_anim_index();
	Command get_type();
private:
	class CommandData {
	public:
		virtual ~CommandData() {}
		virtual Command get_type() = 0;
	};
	class PlayFrame : public CommandData {
	public:
		PlayFrame(int frame, double duration);
		virtual Command get_type() override { return Command::PlayFrame; }
		void update(double dt);
		bool is_frame_done();
		void reset_time();
		int get_frame_num();
		void set_factor(double f);
	private:
		int frame;
		double factor = 1;
		double targetTime;
		double timer;
	};
	class Loop : public CommandData {
	public:
		Loop(int loopToIndex);
		virtual Command get_type() override { return Command::Loop; }
		int get_loop_to_index();
	private:
		int loopToIndex;
	};
	class End : public CommandData {
	public:
		virtual Command get_type() override { return Command::End; }
	private:
	};

	bool isAnimationDone;
	int animSequenceIndex;
	PlayFrame* currPlayFrameData;
	std::vector<CommandData*> animation;
};