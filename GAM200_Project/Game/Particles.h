#pragma once
#include "../Engine/ParticleEmitter.h"

//class AttackParticleEmitter : public ParticleEmitter {
//public:
//    AttackParticleEmitter();
//private:
//    class  AttackParticle : public ParticleEmitter::Particle {
//    public:
//        AttackParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
//    };
//    static constexpr int NumParticles = 10;
//};
//
//class HitParticleEmitter : public ParticleEmitter {
//public:
//    HitParticleEmitter();
//private:
//    class HitParticle : public ParticleEmitter::Particle {
//    public:
//        HitParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
//    };
//    static constexpr int NumParticles = 10;
//};

class StompParticleEmitter : public ParticleEmitter {
public:
    StompParticleEmitter();
private:
    class StompParticle : public ParticleEmitter::Particle {
    public:
        StompParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 10;
};


//class RunParticleEmitter : public ParticleEmitter {
//public:
//    RunParticleEmitter();
//private:
//    class RunParticle : public ParticleEmitter::Particle {
//    public:
//        RunParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
//    };
//    static constexpr int NumParticles = 1;
//};

class JumpParticleEmitter : public ParticleEmitter {
public:
    JumpParticleEmitter();
private:
    class JumpParticle : public ParticleEmitter::Particle {
    public:
        JumpParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 1;
};

class GuardParticleEmitter : public ParticleEmitter {
public:
    GuardParticleEmitter();
private:
    class GuardParticle : public ParticleEmitter::Particle {
    public:
        GuardParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 1;
};


class AdventParticleEmitter : public ParticleEmitter {
public:
    AdventParticleEmitter();
private:
    class AdventParticle : public ParticleEmitter::Particle {
    public:
        AdventParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 1;
};

class ResurrectionParticleEmitter : public ParticleEmitter {
public:
    ResurrectionParticleEmitter();
private:
    class ResurrectionParticle : public ParticleEmitter::Particle {
    public:
        ResurrectionParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 1;
};

class SuperArmorParticleEmitter : public ParticleEmitter {
public:
    SuperArmorParticleEmitter();
private:
    class SuperArmorParticle : public ParticleEmitter::Particle {
    public:
        SuperArmorParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 1;
};


class SpawnSwordParticleEmitter : public ParticleEmitter {
public:
    SpawnSwordParticleEmitter();
private:
    class SpawnSwordParticle : public ParticleEmitter::Particle {
    public:
        SpawnSwordParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 1;
};


class TargetingParticleEmitter : public ParticleEmitter {
public:
    TargetingParticleEmitter();
private:
    class TargetingParticle : public ParticleEmitter::Particle {
    public:
        TargetingParticle(std::string spriteFile) : ParticleEmitter::Particle(spriteFile) {}
    };
    static constexpr int NumParticles = 1;
};