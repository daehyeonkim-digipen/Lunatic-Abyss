#pragma once
#pragma once
#include <vector>
#include "Math.h"
#include "GameObject.h"
#include "Component.h"

class ParticleEmitter : public Component {
protected:
    class Particle : public GameObject {
    public:
        Particle(std::string spriteFile);
        Particle(std::string vertShader, std::string fragShader, math::vec2 size = { 10, 10 });

        void revive(math::vec2 position, math::vec2 velocity, double life, math::vec2 scale, double rotate);
        void update(double dt) override;
        void draw(math::TransformMatrix matrix);

        bool is_alive() { return life > 0; }
    protected:
        double life;
    };
public:
    ParticleEmitter(double lifetime);
    virtual ~ParticleEmitter();
    void add_particle(Particle* particleData);
    void emit(int number, math::vec2 position, math::vec2 emitterVelocity, math::vec2 emitVector, double spread, math::vec2 scale = { 1,1 }, double rotate = 0);
protected:
    std::vector<Particle*> particleMemoryPool;
    int particleIndexToUse;
    double lifetime;
};