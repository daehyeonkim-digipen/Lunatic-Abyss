#pragma once
#include "ParticleEmitter.h"

class AtmoSphericalDustEmitter : public ParticleEmitter {
public:
    AtmoSphericalDustEmitter();

    void update_emit(double dt, int number, math::vec2 position, math::vec2 area_width_height);

    class AtmoSphericalDust : public ParticleEmitter::Particle {
    public:
        AtmoSphericalDust(std::string vertShader, std::string fragShader, math::vec2 size)
            : ParticleEmitter::Particle(vertShader, fragShader, size) {}

        void update(double dt) override;
    };

private:
    double elapsedTime = 0;
    static constexpr double intervalTime = 10;
    static constexpr int NumParticles = 100;
};