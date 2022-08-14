#include "Audio.h"
#include "Engine.h"


Audio::Audio() {
    setup();
}

void Audio::effect_sound_scale_update(float input)
{
    if (input == -10 && effectSound > 0 || input == 10 && effectSound < SFXMaxValue)
    {
        effectSound += input;
    }
}

void Audio::bgm_sound_scale_update(float input)
{
    if (input == -10 && bgmSound > 0 || input == 10 && bgmSound < BGMMaxValue)
    {
        bgmSound += input;
        nowPlayBGM->setVolume(bgmSound);
    }
}

void Audio::load_sound(const std::string& file_path)
{
    SoundBuffers.emplace_back();
    sf::SoundBuffer& buffer = SoundBuffers.back();
    if (!buffer.loadFromFile(file_path))
    {
        Engine::get_logger().log_error("Failed to load sound");
    }
}

void Audio::play_sound(int index)
{
    for (auto& sound : Sounds)
    {
        if (sound.getStatus() != sf::SoundSource::Playing)
        {
            sound.setBuffer(SoundBuffers[index]);
            sound.setVolume(effectSound);
            sound.play();
            return;
        }
    }
    Sounds.emplace_back(SoundBuffers[index]);
    Sounds.back().setVolume(effectSound);
    Sounds.back().play();
}

void Audio::play_bgm(sf::Music* input)
{
    if (nowPlayBGM != input)
    {
        if (nowPlayBGM != nullptr)
        {
            nowPlayBGM->stop();
        }
        nowPlayBGM = input;
        nowPlayBGM->setLoop(true);
        nowPlayBGM->setVolume(bgmSound);
        nowPlayBGM->play();
    }
}

void Audio::setup()
{
    if (!BGMMainMenu.openFromFile("assets/Sound/BGM/BGMMainMenu.ogg")) 
    {
        Engine::get_logger().log_error("Failed to load sound");

    }
    if (!BGMForest.openFromFile("assets/Sound/BGM/BGMForest.ogg"))
    {
        Engine::get_logger().log_error("Failed to load sound");
    }
    //Player
    load_sound("assets/Sound/SFX/Player/Attack.ogg");
    load_sound("assets/Sound/SFX/Player/Charge.ogg");
    load_sound("assets/Sound/SFX/Player/StrongAttack.ogg");
    load_sound("assets/Sound/SFX/Player/Walk1.ogg");
    load_sound("assets/Sound/SFX/Player/Walk2.ogg");
    load_sound("assets/Sound/SFX/Player/Jump.ogg");
    load_sound("assets/Sound/SFX/Player/Land.ogg");
    load_sound("assets/Sound/SFX/Player/Guard.ogg");
    load_sound("assets/Sound/SFX/Player/SuperGuard.ogg");
    load_sound("assets/Sound/SFX/Player/Tackle.ogg");
    load_sound("assets/Sound/SFX/Player/GuardDamaged.ogg");
    load_sound("assets/Sound/SFX/Player/Hurt.ogg");
    load_sound("assets/Sound/SFX/Player/Dead.ogg");
    load_sound("assets/Sound/SFX/Player/Resurrection.ogg");
    //DarkBoar-phase1
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBRoar1.ogg");
	load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBRoar2.ogg");
	load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBRoar3.ogg");
	load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBBite.ogg");
	load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBAttack.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBKick.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBDash.ogg");
    //DarkBoar-phase2
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBRoar4.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBRoar5.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBRoar6.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBPremutate.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBmutate.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBAttack1.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBAttack2.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBSwing.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBPreScream.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBScream.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBPredie.ogg");
    load_sound("assets/Sound/SFX/Enemy/DarkBoar/DBdie.ogg");
    //SkullTree
    load_sound("assets/Sound/SFX/Enemy/SkullTree/STAttack.ogg");
	load_sound("assets/Sound/SFX/Enemy/SkullTree/STGround.ogg");
	load_sound("assets/Sound/SFX/Enemy/SkullTree/STRoot.ogg");
    load_sound("assets/Sound/SFX/Enemy/SkullTree/STPreThrow.ogg");
    load_sound("assets/Sound/SFX/Enemy/SkullTree/STThrowSound.ogg");
	load_sound("assets/Sound/SFX/Enemy/SkullTree/STThrowUp.ogg");
	load_sound("assets/Sound/SFX/Enemy/SkullTree/STThrowMove.ogg");
    load_sound("assets/Sound/SFX/Enemy/SkullTree/STTMHit.ogg");
    load_sound("assets/Sound/SFX/Enemy/SkullTree/STMove.ogg");
    load_sound("assets/Sound/SFX/Enemy/SkullTree/STGather.ogg");
    load_sound("assets/Sound/SFX/Enemy/SkullTree/STDead.ogg");
	//FallenAngel-phase1                         
	load_sound("assets/Sound/SFX/Enemy/FallenAngel/FALaser.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAExplosion.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAWing.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FACrack.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAEarthquake.ogg");
	//FallenAngel-phase2
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAAdvent.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAStoneBroke.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FADescent.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAAttack.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAStrongAttack.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAHit.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAWarpIn.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAWarpOut.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAPrejump.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FAJump.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FADeadEffect.ogg");
    load_sound("assets/Sound/SFX/Enemy/FallenAngel/FADead.ogg");
    //AncientWizard
    load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWAttack.ogg");
	load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWBlackHole.ogg");
	load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWBlindLight.ogg");
	load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWIce.ogg");
	load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWIceBoltHit.ogg");
	load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWLaser.ogg");
	load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWSummon.ogg");
	load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWTeleport.ogg");
    load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWDie.ogg");
    load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWDieEffect.ogg");
    load_sound("assets/Sound/SFX/Enemy/AncientWizard/AWSay.ogg");
	//etc
    load_sound("assets/Sound/SFX/choose.ogg");
    nowPlayBGM = nullptr;
}

void Audio::update(double ) {
    if ((int)prevBgmSound != (int)bgmSound) {
        nowPlayBGM->setVolume(bgmSound);
        prevBgmSound = bgmSound;
    }
}
