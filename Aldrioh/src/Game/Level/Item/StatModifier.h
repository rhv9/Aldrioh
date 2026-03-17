#pragma once

struct StatModifier
{
	float hp_base = 0;
	float hp_multiplier = 0;
	float dmg_base = 0;
	float dmg_multiplier = 0;
	float critChance_base = 0;
	float critChance_multiplier;
	float critDmg_base = 0;
	float critDmg_multiplier = 0;
	float cooldown_base = 0;
	float cooldown_multiplier = 0;
	float luck_modifier = 0;

	StatModifier& operator+=(const StatModifier& other)
	{
		hp_base += other.hp_base;
		hp_multiplier += other.hp_multiplier;
		dmg_base += other.dmg_base;
		dmg_multiplier += other.dmg_multiplier;
		critChance_base += other.critChance_base;
		critChance_multiplier += other.critChance_multiplier;
		critDmg_base += other.critDmg_base;
		critDmg_multiplier += other.critDmg_multiplier;
		cooldown_multiplier += other.cooldown_multiplier;
		cooldown_base += other.cooldown_base;
		luck_modifier += other.luck_modifier;
	}
	
	float CalcHealth() const { return hp_base * (1.0f + hp_multiplier); }
	float CalcDmg() const { return dmg_base * (1.0f + dmg_multiplier); }
	float CalcCritChance() const { return critChance_base * (1.0f + critChance_multiplier); }
	float CalcCritDmg() const { return critDmg_base * (1.0f + critDmg_multiplier); }
	float CalcCooldown() const { return cooldown_base * (1.0f + cooldown_multiplier); }
	float CalcLuck() const { return luck_modifier; }

};