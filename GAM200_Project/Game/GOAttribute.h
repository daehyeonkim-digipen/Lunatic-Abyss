#pragma once
#include "..\Engine\Texture.h" 
#include "..\Engine\Component.h"
#include <string>

class GameObject;
class GOAttribute : public Component {
    friend class StatScene;
public:
    GOAttribute() : GOAttribute(10, 1, 10) {}; // default constructor;
    GOAttribute(double maxHP, double initAD, double maxGage = 0);
    //Set, Update
    void set_max_gage(double input);
    void set_max_hp(double input);
    void set_ad(double input);
    void set_df(double input);
    void set_charge(double input);
    void set_hp(double input);

    void trans(double input);
    void update_gage(double _gage);
    void update_hp(double _hp);
    void update_charge(double input);

    void update(double dt) override;

    //For ingame
    void update_strength(double input);
    void update_health(double input);
    void update_faith(double input);

    void set_ingame_status();
    //Get
    double get_max_gage() const;
	double get_gage() const;
    double get_max_hp() const;
	double get_hp() const;
    double get_ad() const;
    double get_df() const;
    double get_charge() const;

    double get_taken_damage() const;
private:
    double takenDamage;
    double damageStoreTime;

    //Player Ingame Status
    double Strength;
    double Health;
    double Faith;
    
    //Player Max Status
    double maxGage;
    double maxHP;

    //Player Status
    double gage;
	double hp;
    double ad;
    double df;
    double charge;
};