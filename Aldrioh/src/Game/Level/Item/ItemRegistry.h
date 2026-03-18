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
			uint32_t val = 0;
		};
		itemId = k;
		return static_cast<std::size_t>(val);
	}
};


class ItemRegistry
{
public:

	template<typename T>
	T& AddNewItem(const ItemDef& def)
	{
		if (itemMap.find(def.id) != itemMap.end())
		{
			LOG_CRITICAL("ItemRegistry::AddNewItem: Overriding {}", def.id.to_string().c_str());
		}
		itemMap[def.id] = std::make_unique<T>(def);
		itemDefMap[def.id] = def;

		return *static_cast<T*>(itemMap[def.id].get());
	}

	ItemDef& GetItemDef(ItemID itemId);

	std::unique_ptr<Item> CreateInstance(const ItemID& itemId)
	{
		if (itemMap.find(itemId) == itemMap.end())
		{
			LOG_CRITICAL("ItemRegistry::CreateInstance ItemID not found: {}", itemId.to_string());
			// TODO: add a failed item type
		}
		return std::move(itemMap[itemId]->CreateCopy());
	}

	void Debug_PrintRegistry();


private:
	// TODO: this is inefficient?
	std::unordered_map<ItemID, std::unique_ptr<Item>> itemMap;
	std::unordered_map<ItemID, ItemDef> itemDefMap;
};