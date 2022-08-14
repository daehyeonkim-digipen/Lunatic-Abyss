#include "Engine.h"
#include "ShaderEffect.h"

#include "AtmosphericalDustEmitter.h"

#include <random>
#include <ctime> // time(NULL);

AtmoSphericalDustEmitter::AtmoSphericalDustEmitter() : ParticleEmitter(20) {
	std::mt19937 gen((unsigned int)time(NULL));
	std::uniform_int_distribution<int> dis(1, 3); // which dust
	std::uniform_real_distribution<double> dis2(5.0, 65.0); // how big dust

	for (int i = 0; i < AtmoSphericalDustEmitter::NumParticles; i++) {
		int randomParticleIndex = dis(gen);

		std::string particleFileName = "assets/Shaders/dustParticles/normal.frag";
		if (randomParticleIndex == 1)
			particleFileName = "assets/Shaders/dustParticles/bright.frag";
		else if (randomParticleIndex == 2)
			particleFileName = "assets/Shaders/dustParticles/pale.frag";

		double randSize = dis2(gen);

		add_particle(new AtmoSphericalDust("assets/Shaders/e_default.vert", particleFileName, math::vec2{ randSize, randSize }));
	}
}

void AtmoSphericalDustEmitter::update_emit(double dt, int number, math::vec2 position, math::vec2 area) {
	if (elapsedTime >= 0) {
		elapsedTime -= dt;

		if (elapsedTime < 0) {
			std::mt19937 engine((unsigned int)time(NULL));
			std::uniform_real_distribution<double> xPosition(position.x - area.x / 2, position.x + area.x / 2);
			std::uniform_real_distribution<double> yPosition(position.y, position.y + area.y);

			math::vec2 emitterVelocity{ 0.0, 0.1 }; math::vec2 emitVector{ 0.0, 0.1 };
			double spread = 25;

			for (int i = 0; i < number; i++) {
				if (particleMemoryPool[particleIndexToUse]->is_alive() == true) {
					Engine::get_logger().log_error("Particle is being overwritten");
				}

				double angleVariation = spread != 0 ? ((rand() % static_cast<int>(spread * 1024)) / 1024.0f) - spread / 2 : 0;
				math::vec2 particleVelocity = math::RotateMatrix(angleVariation) * emitVector * (((rand() % 1024) / 2048.0f) + 0.5f) + emitterVelocity;

				particleMemoryPool[particleIndexToUse]->revive({ xPosition(engine), yPosition(engine) }, particleVelocity, lifetime, { 1, 1 }, 0);

				particleIndexToUse = (++particleIndexToUse) % static_cast<int>(particleMemoryPool.size());
			}

			elapsedTime = intervalTime;
		}
	}
}

void AtmoSphericalDustEmitter::AtmoSphericalDust::update(double dt) {
	if (is_alive() == true) {
		GameObject::update(dt);
		update_position(get_velocity());

		get_GO_components<ShaderEffect>()->set_variable(Uniforms::DustAlpha, (float)life);

		life -= dt;
	}
}
