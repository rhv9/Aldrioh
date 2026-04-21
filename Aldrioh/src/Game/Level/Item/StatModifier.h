#pragma once

struct StatModifier
{
	float hp_base = 0.0f;
	float hp_multiplier = 0.0f;
	float dmg_base = 0.0f;
	float dmg_multiplier = 0.0f;
	float critChance_base = 0.0f;
	float critChance_multiplier = 0.0f;
	float critDmg_base = 0.0f;
	float critDmg_multiplier = 0.0f;
	float cooldown_base = 0.0f;
	float cooldown_multiplier = 0.0f;
	float luck_modifier = 0.0f;

	float acceleration_base = 0.0f;
	float acceleration_multiplier = 0.0f;
	float maxSpeed_base = 0.0f;
	float maxSpeed_multiplier = 0.0f;

	float movementFalloff_base = 0.0f;
	float movementFalloff_multiplier = 0.0f;

	StatModifier& operator+=(const StatModifier& other)
	{
		hp_base						+= other.hp_base;
		hp_multiplier				+= other.hp_multiplier;

		dmg_base					+= other.dmg_base;
		dmg_multiplier				+= other.dmg_multiplier;

		critChance_base				+= other.critChance_base;
		critChance_multiplier		+= other.critChance_multiplier;

		critDmg_base				+= other.critDmg_base;
		critDmg_multiplier			+= other.critDmg_multiplier;
		
		cooldown_base				+= other.cooldown_base;
		cooldown_multiplier			+= other.cooldown_multiplier;
		
		luck_modifier				+= other.luck_modifier;

		acceleration_base			+= other.acceleration_base;
		acceleration_multiplier		+= other.acceleration_multiplier;

		maxSpeed_base				+= other.maxSpeed_base;
		maxSpeed_multiplier			+= other.maxSpeed_multiplier;

		movementFalloff_base		+= other.movementFalloff_base;
		movementFalloff_multiplier	+= other.movementFalloff_multiplier;
		return *this;
	}
	
	float CalcHealth() const { return hp_base * (hp_multiplier + 1.0f); }
	float CalcDmg() const { return dmg_base * (dmg_multiplier + 1.0f); }
	float CalcCritChance() const { return critChance_base * (critChance_multiplier + 1.0f); }
	float CalcCritDmg() const { return critDmg_base * (critDmg_multiplier + 1.0f); }
	float CalcCooldown() const { return cooldown_base * (cooldown_multiplier); }
	float CalcLuck() const { return luck_modifier; }
	float CalcAcceleration() const { return acceleration_base * (acceleration_multiplier + 1.0f); }
	float CalcMaxSpeed() const { return maxSpeed_base * (maxSpeed_multiplier + 1.0f); }
	float CalcMovementFalloff() const { return movementFalloff_base * (movementFalloff_multiplier + 1.0f); }

};