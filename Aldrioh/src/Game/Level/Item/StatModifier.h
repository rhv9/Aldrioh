#pragma once

struct StatModifier
{
	int hp_base = 0;
	int hp_multiplier = 0;
	int dmg_base = 0;
	int dmg_multiplier = 0;
	int critChance_base = 0;
	int critChance_multiplier = 0;
	int critDmg_base = 0;
	int critDmg_multiplier = 0;
	int cooldown_base = 0;
	int cooldown_multiplier = 0;
	int luck_modifier = 0;

	StatModifier& operator+=(const StatModifier& other)
	{
		hp_base					+= other.hp_base;
		hp_multiplier			+= other.hp_multiplier;

		dmg_base				+= other.dmg_base;
		dmg_multiplier			+= other.dmg_multiplier;

		critChance_base			+= other.critChance_base;
		critChance_multiplier	+= other.critChance_multiplier;

		critDmg_base			+= other.critDmg_base;
		critDmg_multiplier		+= other.critDmg_multiplier;
		
		cooldown_base			+= other.cooldown_base;
		cooldown_multiplier		+= other.cooldown_multiplier;
		
		luck_modifier			+= other.luck_modifier;
		return *this;
	}
	
	float CalcHealth() const { return hp_base * (hp_multiplier / 100.0f); }
	float CalcDmg() const { return dmg_base * (dmg_multiplier / 100.0f); }
	float CalcCritChance() const { return critChance_base * (critChance_multiplier / 100.0f); }
	float CalcCritDmg() const { return critDmg_base * (critDmg_multiplier / 100.0f); }
	float CalcCooldown() const { return cooldown_base * (cooldown_multiplier / 100.0f); }
	float CalcLuck() const { return luck_modifier / 100.0f; }

};