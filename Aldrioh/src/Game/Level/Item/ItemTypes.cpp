#include <pch.h>
#include "ItemTypes.h"
#include "ItemRegistry.h"
#include <Game/SpriteCollection.h>

void ItemTypes::Init(ItemRegistry& itemRegistry)
{
	std::unique_ptr<BaseStatItem> baseStat_Hp = std::make_unique<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Hp, "HP Part", "Increase your HP", Sprites::asteroid_small });
}
