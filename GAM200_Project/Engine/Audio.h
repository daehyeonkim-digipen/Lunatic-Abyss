#pragma once
#include "Component.h"
#include <SFML/Audio.hpp>

struct Audio : public Component
{
private:
    std::vector<sf::SoundBuffer>  SoundBuffers{};
    std::vector<sf::Sound>        Sounds{};
    sf::Music* nowPlayBGM = nullptr;
public:
    Audio();
    sf::Music                     BGMMainMenu;
    sf::Music                     BGMForest;

    void effect_sound_scale_update(float input);
    void bgm_sound_scale_update(float input);

    void load_sound(const std::string& file_path);
    void play_sound(int index);
    void play_bgm(sf::Music* input);
    void setup();

    void update(double dt) override;

    float& get_effect_sound() { return effectSound; }
    float& get_bgm_sound() { return bgmSound; }
    enum class Effects 
    {
        //Player
        Attack,
        Charge,
        SuperAttack,
        Walk1,
        Walk2,
        Jump,
        Land,
        Guard,
        SuperGuard,
        Tackle,
        GuardDamaged,
        Hurt,
        Dead,
        Resurrection,
        //DarkBoar-phase1
        DBRoar1,
        DBRoar2,
        DBRoar3,
        DBBite,
        DBAttack,
        DBKick,
        DBDash,
        //DarkBoar-phase2
        DBRoar4,
        DBRoar5,
        DBRoar6,
        DBPremutate,
        DBMutate,
        DBAttack1,
        DBAttack2,
        DBSwing,
        DBPreScream,
        DBScream,
        DBPredie,
        DBDie,
        //SkullTree
        STAttack,
        STGround,
        STRoot,
        STPreThrow,
        STThrowSound,
        STThrowUp,
        STThrowMove,
        STTMHit,
        STMove,
        STGather,
        STDead,
        //FallenAngel-phase1
    	FALaser,
        FAExplosion,
        FAWing,
        FACrack,
        FAEarthquake,
        //FallenAngel-phase2
        FAAdvent,
        FAStoneBroke,
        FADescent,
        FAAttack,
        FAStrongAttack,
        FAHit,
        FAWarpIn,
        FAWarpOut,
        FAPrejump,
        FAJump,
        FADeadEffect,
    	FADead,
        //
    	AWAttack,
        AWBlackHole,
        AWBlindLight,
        AWIce,
        AWIceBoltHit,
        AWLaser,
        AWSummon,
        AWTeleport,
        AWDie,
        AWDieEffect,
        AWSay,
    	//etc
        Choose,
        Count
    };

    static constexpr double BGMMaxValue = 100;
    static constexpr double SFXMaxValue = 100;
private:
    float effectSound = 50;
    float bgmSound = 50;
    float prevBgmSound = bgmSound;
};