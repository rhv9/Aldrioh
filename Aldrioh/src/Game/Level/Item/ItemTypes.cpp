#include <pch.h>
#include "ItemTypes.h"
#include "ItemRegistry.h"
#include <Game/SpriteCollection.h>

void ItemTypes::Init(ItemRegistry& itemRegistry)
{
	auto& baseStat_Hp = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Hp, "HP Gear", "Increases your HP", Sprites::asteroid_small });
	baseStat_Hp.statModifier.hp_multiplier = 0.1f;
	baseStat_Hp.levelUpFunc = [](Item* itemPtr) -> std::string
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			float old = item->statModifier.hp_multiplier;
			item->statModifier.hp_multiplier += 0.1f;
			return "Increase HP by 10%";
		};

	auto& baseStat_dmg = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Dmg, "Damage Gear", "Increases your damage", Sprites::asteroid_small });
	baseStat_dmg.statModifier.dmg_multiplier = 0.1f;
	baseStat_dmg.levelUpFunc = [](Item* itemPtr) -> std::string
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			float old = item->statModifier.dmg_multiplier;
			item->statModifier.dmg_multiplier += 0.1f;
			return "Increase damage by 10%";
		};

	auto& baseStat_critChance = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_CritChance, "Crit Chance Gear", "Increases your chance of a critical hit", Sprites::asteroid_small });
	baseStat_critChance.statModifier.critChange_multiplier = 0.1f;
	baseStat_critChance.levelUpFunc = [](Item* itemPtr) -> std::string
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			float old = item->statModifier.critChange_multiplier;
			item->statModifier.critChange_multiplier += 0.1f;
			return "Increase critical chance by 10%";
		};

	auto& baseStat_critDamage = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_CritDamage, "Crit Damage Gear", "Increases your critical hit damage", Sprites::asteroid_small });
	baseStat_critDamage.statModifier.critDmg_multiplier = 0.1f;
	baseStat_critDamage.levelUpFunc = [](Item* itemPtr) -> std::string
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			float old = item->statModifier.critDmg_multiplier;
			item->statModifier.critDmg_multiplier += 0.1f;
			return "Increase critical damage by 10%";
		};

	auto& baseStat_luck = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Luck, "Clover Gear", "Increases your luck", Sprites::asteroid_small });
	baseStat_luck.statModifier.luck_modifier = 0.1f;
	baseStat_luck.levelUpFunc = [](Item* itemPtr) -> std::string
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			float old = item->statModifier.luck_modifier;
			item->statModifier.luck_modifier += 0.1f;
			return "Increase your luck by 10%";
		};

	itemRegistry.Debug_PrintRegistry();
}
