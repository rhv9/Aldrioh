#pragma once

#include "ItemTypes.h"

class ItemPool
{
public:
	ItemPool();

	std::array<ItemID, 3> GenerateThreeUniqueItems();

private:
	std::vector<ItemID> itemPool;
};