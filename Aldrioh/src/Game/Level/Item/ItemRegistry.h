#pragma once
#include "Item.h"

class ItemRegistry
{
public:
	void Add(std::unique_ptr<Item> itemPtr);
	Item* Get(ItemID id);

private:
	constexpr static int NUM_ITEMS = 64;
	std::array<std::unique_ptr<Item>, static_cast<int>(ItemCategory::Count) * NUM_ITEMS> itemArray;
};