#pragma once
#include <Game/SpriteCollection.h>

using ItemID = uint16_t;

struct ItemDef
{
	ItemID id;
	std::string_view name;
	std::string_view description;
	spriteid_t spriteId;
};

class Item
{
public:
	Item(ItemID id)
		: id(id) {
	}

	virtual std::string IncreaseLevel() = 0;
protected:
	ItemID id;
	int lvl;
};

class BaseStatItem : public Item
{
public:
	BaseStatItem(ItemID id)
		: Item(id) {
	}

};

class ShipModuleItem : public Item
{

};