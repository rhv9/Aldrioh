#pragma once
#include <Game/SpriteCollection.h>
#include "StatModifier.h"

using ItemID = uint16_t;

struct ItemDef
{
	ItemID id;
	std::string_view name;
	std::string_view description;
	spriteid_t spriteId;
};

struct Item
{
	using increase_tfunc = std::function<std::string(void)>;

	increase_tfunc levelUpFunc;
	ItemID id;
	spriteid_t cachedSpriteId;
	int lvl;

	Item(ItemID id) : id(id) {}
};

struct BaseStatItem : public Item
{
	BaseStatItem(ItemID id) : Item(id) {}

	StatModifier statModifier;
};

class ShipModuleItem : public Item
{

};