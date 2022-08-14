#include "Engine.h"
#include "Logger.h"
#include "SceneManager.h"
#include "TextureManager.h"
#include "GameObjectManager.h"
#include "Game.h"
#include "Input.h"
#include "Audio.h"
#include "Graphic.h"
#include "AtmosphericalDustEmitter.h"

#include "../Game/Particles.h"

float Engine::ElpasedTime = 0;

Engine::Engine() : frameCount(0)
{}

Engine::~Engine()
{
    clear_GE_components();
	glfwTerminate();
}

void Engine::init(const std::string& title)
{
    fpsCalcTime = lastTick;

    gameEngineComponentManager.add_component(new Input());
    gameEngineComponentManager.add_component(new IGL::Graphic(title));
    gameEngineComponentManager.add_component(new Audio());
    gameEngineComponentManager.add_component(new TextureManager());
    gameEngineComponentManager.add_component(new Game());

    gameEngineComponentManager.add_component(new GameObjectManager()); // particles

    //gameEngineComponentManager.add_component(new AttackParticleEmitter());
    //gameEngineComponentManager.add_component(new HitParticleEmitter());
    gameEngineComponentManager.add_component(new StompParticleEmitter());
    //gameEngineComponentManager.add_component(new RunParticleEmitter());
    gameEngineComponentManager.add_component(new JumpParticleEmitter());
    gameEngineComponentManager.add_component(new GuardParticleEmitter());
    //gameEngineComponentManager.add_component(new AtmoSphericalDustEmitter());
    gameEngineComponentManager.add_component(new AdventParticleEmitter());
    gameEngineComponentManager.add_component(new ResurrectionParticleEmitter());
    gameEngineComponentManager.add_component(new SuperArmorParticleEmitter());
    gameEngineComponentManager.add_component(new SpawnSwordParticleEmitter());
    gameEngineComponentManager.add_component(new TargetingParticleEmitter());


    auto seed = (unsigned int)time(NULL);
    srand(seed);
    get_logger().log_debug("Seed : " + std::to_string(seed));

}

void Engine::update()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    double dt = std::chrono::duration<double>(now - lastTick).count();

    if(dt > 2. / Engine::FramePurpose) { // frame lock
        dt = 1 / Engine::FramePurpose; 
    }
    if (dt >= 1 / Engine::FramePurpose) {
        lastTick = now;
        ElpasedTime += static_cast<float>(dt);

        if (++frameCount >= 300) {
            double actualTime = std::chrono::duration<double>((now - fpsCalcTime)).count();
            logger.log_debug("FPS:  " + std::to_string(frameCount / actualTime));
            frameCount = 0;
            fpsCalcTime = now;
        }

        gameEngineComponentManager.update_all(dt);
        gameEngineComponentManager.get_component<IGL::Graphic>()->final();
    }
}

bool Engine::has_game_ended()
{
    SceneManager* scm = get_GE_component<Game>()->get_game_component<SceneManager>();

    if (scm == nullptr) {
        return true;
    }

	return scm->has_scene_ended();
}