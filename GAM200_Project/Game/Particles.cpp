#include "Particles.h"

//AttackParticleEmitter::AttackParticleEmitter() : ParticleEmitter(0.3) {
//	for (int i = 0; i < AttackParticleEmitter::NumParticles; i++) {
//		add_particle(new AttackParticle("assets/Sprite/Particles/attackParticle.spt"));
//	}
//}
//
//HitParticleEmitter::HitParticleEmitter() : ParticleEmitter(0.4) {
//	for (int i = 0; i < HitParticleEmitter::NumParticles; i++) {
//		add_particle(new HitParticle("assets/Sprite/Particles/hitParticle.spt"));
//	}
//}

StompParticleEmitter::StompParticleEmitter() : ParticleEmitter(0.3) {
	for (int i = 0; i < StompParticleEmitter::NumParticles; i++) {
		add_particle(new StompParticle("assets/Sprite/Particles/StompParticle.spt"));
	}
}

//RunParticleEmitter::RunParticleEmitter() : ParticleEmitter(0.3) {
//	for (int i = 0; i < RunParticleEmitter::NumParticles; i++) {
//		add_particle(new RunParticle("assets/Sprite/Particles/RunParticle.spt"));
//	}
//}

JumpParticleEmitter::JumpParticleEmitter() : ParticleEmitter(0.3) {
	for (int i = 0; i < JumpParticleEmitter::NumParticles; i++) {
		add_particle(new JumpParticle("assets/Sprite/Particles/JumpParticle.spt"));
	}
}


GuardParticleEmitter::GuardParticleEmitter() : ParticleEmitter(0.3) {
	for (int i = 0; i < GuardParticleEmitter::NumParticles; i++) {
		add_particle(new GuardParticle("assets/Sprite/Particles/GuardParticle.spt"));
	}
}

AdventParticleEmitter::AdventParticleEmitter() : ParticleEmitter(2.2)
{
	for (int i = 0; i < AdventParticleEmitter::NumParticles; i++) {
		add_particle(new AdventParticle("assets/Sprite/Particles/AdventParticle.spt"));
	}
}

ResurrectionParticleEmitter::ResurrectionParticleEmitter() : ParticleEmitter(1)
{
	for (int i = 0; i < ResurrectionParticleEmitter::NumParticles; i++) {
		add_particle(new ResurrectionParticle("assets/Sprite/Particles/rebirth.spt"));
	}
}

SuperArmorParticleEmitter::SuperArmorParticleEmitter() : ParticleEmitter(1)
{
	for (int i = 0; i < SuperArmorParticleEmitter::NumParticles; i++) {
		add_particle(new SuperArmorParticle("assets/Sprite/Particles/shield.spt"));
	}
}

SpawnSwordParticleEmitter::SpawnSwordParticleEmitter() : ParticleEmitter(1.7)
{
	for (int i = 0; i < SpawnSwordParticleEmitter::NumParticles; i++) {
		add_particle(new SpawnSwordParticle("assets/Sprite/Particles/magic_sword.spt"));
	}
}


TargetingParticleEmitter::TargetingParticleEmitter() : ParticleEmitter(1.2)
{
	for (int i = 0; i < TargetingParticleEmitter::NumParticles; i++) {
		add_particle(new TargetingParticle("assets/Sprite/Particles/targeting.spt"));
	}
}
