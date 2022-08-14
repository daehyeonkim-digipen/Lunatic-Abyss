#pragma once
#include "ComponentManager.h"
#include "Logger.h"

#include <chrono>

class Engine
{
public:
	static Engine& instance() { static Engine instance; return instance; }
	static Logger& get_logger() { return instance().logger; } 

	template <typename T>
	static T* get_GE_component() { return instance().gameEngineComponentManager.get_component<T>(); }
	static void clear_GE_components() { 
		instance().gameEngineComponentManager.clear_components();
	}
	
	void init(const std::string&);
	void update();
	bool has_game_ended();

	static float ElpasedTime;

private:

	ComponentManager gameEngineComponentManager; // GE
	Logger logger;

	Engine();
	~Engine();

private: // clock
	std::chrono::system_clock::time_point lastTick;
	std::chrono::system_clock::time_point fpsCalcTime;
	int frameCount;

	static constexpr double FramePurpose = 144.;
};




