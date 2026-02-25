#include <pch.h>
#include "ItemTypes.h"
#include "ItemRegistry.h"
#include <Game/SpriteCollection.h>

void ItemTypes::Init(ItemRegistry& itemRegistry)
{
	auto& baseStat_Hp = itemRegistry.CreateItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Hp, "HP Part", "Increase your HP", Sprites::asteroid_small });
	baseStat_Hp.statModifier.hp_multiplier = 0.1f;
	baseStat_Hp.levelUpFunc = [](Item* itemPtr) -> std::string
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			float old = item->statModifier.hp_multiplier;
			item->statModifier.hp_multiplier += 0.1f;
			return "Increase HP by 10%";
		};


	itemRegistry.Debug_PrintRegistry();
}
