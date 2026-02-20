#pragma once
#include <Game/SpriteCollection.h>
#include "StatModifier.h"

class ItemRegistry;

using itemid_t = uint16_t;
enum class ItemCategory : uint8_t
{
	BaseStat = 0,
	ShipModule,
	Special,
	Count
};

class Entity;

struct ItemID
{
	itemid_t id;
	ItemCategory category;

	bool operator==(const ItemID& other) 
	{
		return id == other.id && category == other.category;
	}
};

struct ItemDef
{
	ItemID id;
	std::string_view name;
	std::string_view description;
	spriteid_t spriteId;
};

struct Item
{
	using levelup_tfunc = std::function<std::string(void)>;

	ItemID id;
	spriteid_t cachedSpriteId;
	int lvl = 1;

	levelup_tfunc levelUpFunc;

	Item(ItemRegistry& itemRegistry, const ItemDef& def);
};

struct BaseStatItem : public Item
{
	StatModifier statModifier;

	BaseStatItem(const ItemDef& def) : Item(def) {}
};

class ShipModuleItem : public Item
{
	using update_tfunc = std::function<void(Timestep ts, Entity& e)>;

	update_tfunc updateFunc;

	ShipModuleItem(const ItemDef& def) : Item(def) {}
};