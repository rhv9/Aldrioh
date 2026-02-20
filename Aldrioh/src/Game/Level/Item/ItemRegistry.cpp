#include <pch.h>
#include "ItemRegistry.h"

void ItemRegistry::Add(std::unique_ptr<Item> itemPtr)
{
	int index = static_cast<int>(itemPtr->id.category) * NUM_ITEMS + itemPtr->id.id;
	if (!itemArray[index])
		LOG_CRITICAL("Adding an item index that is already added!");
	itemArray[index] = std::move(itemPtr);
}

Item* ItemRegistry::Get(ItemID id)
{
	int index = static_cast<int>(id.category) * NUM_ITEMS + id.id;
	if (!itemArray[index])
	{
		LOG_CRITICAL("Item does not exist, returning nullptr!");
		return nullptr;
	}
	return itemArray[index].get();
}
