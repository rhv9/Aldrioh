#pragma once
#include <Game/SpriteCollection.h>
#include "StatModifier.h"
#include <Scene/Entity.h>

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

class Item
{
public:
	Item() = default;
	Item(const ItemDef& def) : id(def.id), cachedSpriteId(def.spriteId) {}
	
	virtual std::unique_ptr<Item> CreateCopy() = 0;

	virtual LvlUpInfo LevelUp() = 0;
	virtual LvlUpInfo LevelUpPretend() = 0;

	ItemID id;
	spriteid_t cachedSpriteId;
	int lvl = 1;
};

class BaseStatItem : public Item
{
public:
	using levelup_tfunc = std::function<LvlUpInfo(Item*)>;
	BaseStatItem() = default;
	BaseStatItem(const ItemDef& def) : Item(def) {}
	virtual std::unique_ptr<Item> CreateCopy() { return std::make_unique<BaseStatItem>(*this); }

	virtual LvlUpInfo LevelUp() override { return levelUpFunc(this); }
	virtual LvlUpInfo LevelUpPretend() { BaseStatItem copyItem = *this; return copyItem.LevelUp(); }

	StatModifier statModifier;
	levelup_tfunc levelUpFunc;
};

class ShipModuleItem : public Item
{
public:
	ShipModuleItem() = default;
	ShipModuleItem(const ItemDef& def) : Item(def) {}
	virtual std::unique_ptr<Item> CreateCopy() = 0;

	virtual LvlUpInfo LevelUp() = 0;
	virtual LvlUpInfo LevelUpPretend() = 0;
	virtual void OnUpdate(Timestep ts, Entity e) {}
};

class UniqueItem : public Item
{
public:
	using update_tfunc = std::function<void(Timestep ts, Entity e)>;
	UniqueItem() = default;
	UniqueItem(const ItemDef& def) : Item(def) {}
	virtual std::unique_ptr<Item> CreateCopy() { return std::make_unique<UniqueItem>(*this); }

	virtual LvlUpInfo LevelUp() { return { "This should never run!" }; };
	virtual LvlUpInfo LevelUpPretend() { UniqueItem copyItem = *this; return copyItem.LevelUp(); }
	virtual void OnUpdate(Timestep ts, Entity e) {}
};