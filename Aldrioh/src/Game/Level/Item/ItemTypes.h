#pragma once
#include "Item.h"

class ItemRegistry;

namespace ItemTypes
{
	void Init(ItemRegistry& itemRegistry);

	constexpr ItemID BaseStat_Hp			{ 1, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_Dmg			{ 2, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_CritChance	{ 3, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_CritDamage	{ 4, ItemCategory::BaseStat };
	constexpr ItemID BaseStat_Luck			{ 5, ItemCategory::BaseStat };

	constexpr ItemID ShipModule_Shooter		{ 1, ItemCategory::ShipModule };
	constexpr ItemID ShipModule_Rocket		{ 2, ItemCategory::ShipModule };
	constexpr ItemID ShipModule_AOEDrone	{ 3, ItemCategory::ShipModule };
	constexpr ItemID ShipModule_MachineGun	{ 4, ItemCategory::ShipModule };
	
	constexpr ItemID Unique_FloatyFast		{ 1, ItemCategory::Unique };


}