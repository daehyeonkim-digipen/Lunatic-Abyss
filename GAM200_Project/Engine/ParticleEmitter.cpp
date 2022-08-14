#include "Engine.h"
#include "GameObjectManager.h"
#include "ParticleEmitter.h"
#include "Sprite.h"
#include "ShaderEffect.h"
#include "Game.h"

ParticleEmitter::ParticleEmitter(double lifetime) : particleIndexToUse(0), lifetime(lifetime) {}

ParticleEmitter::~ParticleEmitter() { particleMemoryPool.clear(); }


void ParticleEmitter::add_particle(ParticleEmitter::Particle* particleData) {
    particleMemoryPool.push_back(particleData);
    Engine::get_GE_component<GameObjectManager>()->add_game_object(ObjType::Particle, particleData);
}

void ParticleEmitter::emit(int toEmit, math::vec2 emitterPosition, math::vec2 emitterVelocity, math::vec2 emitVector, double spread, math::vec2 scale, double rotate) {
    for (int i = 0; i < toEmit; i++) {
        if (particleMemoryPool[particleIndexToUse]->is_alive() == true) {
            Engine::get_logger().log_error("Particle is being overwritten");
        }

        double angleVariation = spread != 0 ? ((rand() % static_cast<int>(spread * 1024)) / 1024.0f) - spread / 2 : 0;
        math::vec2 particleVelocity = math::RotateMatrix(angleVariation) * emitVector * (((rand() % 1024) / 2048.0f) + 0.5f) + emitterVelocity;
        particleMemoryPool[particleIndexToUse++]->revive(emitterPosition, particleVelocity, lifetime, scale, rotate);
        if (particleIndexToUse >= particleMemoryPool.size()) {
            particleIndexToUse = 0;
        }
    }
}

ParticleEmitter::Particle::Particle(std::string spriteFile) : GameObject({ 0,0 }), life(0) 
{
    set_type(GOType::particle);
    add_GO_component(new Sprite(spriteFile, this));
}

ParticleEmitter::Particle::Particle(std::string vertShader, std::string fragShader, math::vec2 size) : GameObject({ 0,0 }), life(0) {
    set_type(GOType::particle);
    add_GO_component(new ShaderEffect(CustomShader(vertShader, fragShader), size));
}

void ParticleEmitter::Particle::revive(math::vec2 newPosition, math::vec2 newVelocity, double newLife, math::vec2 newScale, double newAngle) {
    life = newLife;
    set_position(newPosition);
    set_scale(newScale);
    set_velocity(newVelocity);
    set_rotation(newAngle);
    if (get_GO_components<Sprite>() != nullptr)
        get_GO_components<Sprite>()->play_animation(0);
}

void ParticleEmitter::Particle::update(double dt) {
    if (is_alive() == true) {
        life -= dt;
        GameObject::update(dt);
        //update_position(get_velocity());
    }
}

void ParticleEmitter::Particle::draw(math::TransformMatrix matrix) {
    if (is_alive() == true) {
        GameObject::draw(matrix);
    }
}