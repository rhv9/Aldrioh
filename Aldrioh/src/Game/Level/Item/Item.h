#pragma once
#include <Game/SpriteCollection.h>
#include "StatModifier.h"

class ItemRegistry;
class Entity;

using itemid_t = uint16_t;

enum ItemCategory : uint8_t
{
	None = 0,
	BaseStat,
	ShipModule,
	Unique,
	Count
};


struct ItemID
{
	itemid_t id = 0;
	ItemCategory category = None;

	bool operator==(const ItemID& other) const { return id == other.id && category == other.category; }
	std::string to_string() const { return std::format("ItemID({},{})", id, static_cast<uint8_t>(category)); }

	bool IsValid() { return category != ItemCategory::None; }
};


struct ItemDef
{
	ItemID id;
	std::string name;
	std::string description;
	spriteid_t spriteId = Sprites::null;
};

struct LvlUpInfo
{
	std::string msg;
};

struct Item
{
	using levelup_tfunc = std::function<LvlUpInfo(Item*)>;

	ItemID id;
	spriteid_t cachedSpriteId;
	int lvl = 1;

	levelup_tfunc levelUpFunc;

	virtual LvlUpInfo LevelUpPretend() { Item copyItem = *this; return copyItem.LevelUp(); }
	LvlUpInfo LevelUp() { return levelUpFunc(this); }

	Item() = default;
	Item(const ItemDef& def) : id(def.id), cachedSpriteId(def.spriteId) {}
};

struct BaseStatItem : public Item
{
	StatModifier statModifier;

	virtual LvlUpInfo LevelUpPretend() { BaseStatItem copyItem = *this; return copyItem.LevelUp(); }

	BaseStatItem() = default;
	BaseStatItem(const ItemDef& def) : Item(def) {}
};

struct ShipModuleItem : public Item
{
	using update_tfunc = std::function<void(Timestep ts, Entity& e)>;

	update_tfunc updateFunc;

	virtual LvlUpInfo LevelUpPretend() { ShipModuleItem copyItem = *this; return copyItem.LevelUp(); }

	ShipModuleItem() = default;
	ShipModuleItem(const ItemDef& def) : Item(def) {}
};

struct UniqueItem : public Item
{
	using update_tfunc = std::function<void(Timestep ts, Entity& e)>;

	update_tfunc updateFunc;

	virtual LvlUpInfo LevelUpPretend() { UniqueItem copyItem = *this; return copyItem.LevelUp(); }

	UniqueItem() = default;
	UniqueItem(const ItemDef& def) : Item(def) {}
};