#pragma once
#include "GOAttribute.h"
#include <fstream>

GOAttribute::GOAttribute(double maxHP, double initAD, double maxGage)
	: maxHP(maxHP), hp(maxHP), ad(initAD), maxGage(maxGage), gage(0), df(0), charge(0), Strength(5), Faith(5), Health(5)
{
}



void GOAttribute::update_strength(double input)
{
	Strength += input;
}

void GOAttribute::update_health(double input)
{
	Health += input;
}

void GOAttribute::update_faith(double input)
{
	Faith += input;
}


void GOAttribute::set_max_gage(double input)
{
	maxGage = input;
}

void GOAttribute::set_max_hp(double input)
{
	maxHP = input;
}

void GOAttribute::set_ad(double input)
{
	ad = input;
}

void GOAttribute::set_df(double input)
{
	df = input;
}

void GOAttribute::set_charge(double input)
{
	charge = input;
}

void GOAttribute::set_hp(double input)
{
	hp = input;
}

void GOAttribute::trans(double input)
{
	if(gage > 0)
	{
		update_gage(-input);
		update_hp(input / 2.);
	}
}

void GOAttribute::update_charge(double input)
{
	charge += input;
	if (charge <= 0) {
		charge = 0;
	}
}

void GOAttribute::update(double dt) {
	if (damageStoreTime >= 0) {
		damageStoreTime -= dt;
	}
	else {
		takenDamage = 0;
	}
}

void GOAttribute::set_ingame_status()
{
	ad = Strength * 3;
	maxHP =  Health * 20;
	hp = maxHP;
	maxGage = Faith * 10;
}

void GOAttribute::update_gage(double _gage)
{
	if(_gage > 0)
	{
		gage += _gage * (0.2 * Faith);
	} else
	{
		gage += _gage;
	}
	if (gage <= 0) {
		gage = 0;
	}
	if (gage > maxGage)
	{
		gage = maxGage;
	}
}

void GOAttribute::update_hp(double _hp)
{
	if (_hp < 0) {
		takenDamage += abs(_hp);
		damageStoreTime = 1.5;
	}

	if(_hp > 0)
	{
		hp += _hp * (0.2 * Health);
	} else
	{
		hp += _hp * (1. - df);
	}
	if (hp <= 0) {
		hp = 0;
	}
	if (hp > maxHP)
	{
		hp = maxHP;
	}
}

double GOAttribute::get_max_gage() const
{
	return maxGage;
}

double GOAttribute::get_gage() const
{
	return gage;
}

double GOAttribute::get_max_hp() const
{
	return maxHP;
}

double GOAttribute::get_hp() const
{
	return hp;
}

double GOAttribute::get_ad() const
{
	return ad;
}

double GOAttribute::get_df() const
{
	return df;
}

double GOAttribute::get_charge() const
{
	return charge;
}

double GOAttribute::get_taken_damage() const
{
	return takenDamage;
}
