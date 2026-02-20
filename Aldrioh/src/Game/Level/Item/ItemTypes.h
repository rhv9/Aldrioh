#pragma once
#include "Item.h"

class ItemRegistry;

namespace ItemTypes
{
	void Init(ItemRegistry& itemRegistry);

	constexpr ItemID BaseStat_Hp			{ 0, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_Dmg			{ 1, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_CritChance	{ 2, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_CritDamage	{ 3, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_Luck			{ 4, ItemCategory::BaseStat };

	constexpr ItemID ShipModule_Shooter		{ 0, ItemCategory::ShipModule };
	constexpr ItemID ShipModule_Rocket		{ 1, ItemCategory::ShipModule };
	constexpr ItemID ShipModule_AOEDrone	{ 2, ItemCategory::ShipModule };
	
	constexpr ItemID Special_DoubleDmg		{ 0, ItemCategory::Special };

}