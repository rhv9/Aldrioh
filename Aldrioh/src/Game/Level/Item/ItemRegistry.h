#pragma once
#include "Item.h"

template<>
struct std::hash<ItemID>
{
	std::size_t operator()(const ItemID& k) const
	{
		union
		{
			ItemID itemId;
			uint32_t val;
		};
		itemId = k;
		return static_cast<std::size_t>(val);
	}
};


class ItemRegistry
{
public:
	
	template<typename T>
	std::unique_ptr<T>& CreateItem(const ItemDef& def)
	{
		if (itemMap.find(def.id) != itemMap.end())
		{
			LOG_CRITICAL("ItemRegistry::AddNewItem: Overriding {}", def.id.to_string().c_str());
		}
		itemMap[def.id] = std::make_unique<T>(def);
		itemDefMap[def.id] = def;
	}


private:
	// TODO: this is inefficient?
	std::unordered_map<ItemID, std::unique_ptr<Item>> itemMap;
	std::unordered_map<ItemID, ItemDef> itemDefMap;
};