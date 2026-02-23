#pragma once
#include <Game/SpriteCollection.h>
#include "StatModifier.h"

class ItemRegistry;
class Entity;

using itemid_t = uint16_t;

enum ItemCategory : uint8_t
{
	BaseStat = 0,
	ShipModule,
	Unique,
	Count
};

struct ItemID
{
	itemid_t id;
	ItemCategory category;

	bool operator==(const ItemID& other) const { return id == other.id && category == other.category; }
	std::string to_string() const { return std::format("ItemID({},{})", id, static_cast<uint8_t>(category)); }
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
	using levelup_tfunc = std::function<std::string(Item*)>;

	ItemID id;
	spriteid_t cachedSpriteId;
	int lvl = 1;

	levelup_tfunc levelUpFunc;

	Item(const ItemDef& def) : id(def.id), cachedSpriteId(def.spriteId) {}
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

class UniqueItem : public Item
{
	using update_tfunc = std::function<void(Timestep ts, Entity& e)>;

	update_tfunc updateFunc;

	UniqueItem(const ItemDef& def) : Item(def) {}
};