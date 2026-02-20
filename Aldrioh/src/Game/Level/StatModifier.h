#pragma once

struct StatModifier
{
	float hp_base = 0;
	float hp_multiplier = 0;
	float dmg_base = 0;
	float dmg_multiplier = 0;
	float critChance_base = 0;
	float critChange_multiplier;
	float critDmg_base = 0;
	float critDmg_multiplier = 0;
	float cooldown_multiplier = 0;
	float luck_modifier = 0;

	StatModifier& operator+=(const StatModifier& other)
	{
		hp_base += other.hp_base;
		hp_multiplier += other.hp_multiplier;
		dmg_base += other.dmg_base;
		dmg_multiplier += other.dmg_multiplier;
		critChance_base += other.critChance_base;
		critChange_multiplier += other.critChange_multiplier;
		critDmg_base += other.critDmg_base;
		critDmg_multiplier += other.critDmg_multiplier;
		cooldown_multiplier += other.cooldown_multiplier;
		luck_modifier += other.luck_modifier;
	}
};