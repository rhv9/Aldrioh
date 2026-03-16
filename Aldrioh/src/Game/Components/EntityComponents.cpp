#include <pch.h>
#include "EntityComponents.h"

#include <Game/GlobalResource.h>

Item* arrayGetItem(auto& items, const auto elem, auto itemCount)
{
	for (int i = 0; i < itemCount; ++i)
		if (items[i].id == elem)
			return &items[i];
	return nullptr;
}

bool ModularShipComponent::HasItem(const ItemID itemId)
{
	return GetItem(itemId) != nullptr;
}

Item* ModularShipComponent::GetItem(const ItemID itemId)
{
	switch (itemId.category)
	{
	case ItemCategory::BaseStat:
		return arrayGetItem(bsi, itemId, bsiCount);
	case ItemCategory::ShipModule:
		return arrayGetItem(smi, itemId, smiCount);
	case ItemCategory::Unique:
		return arrayGetItem(si, itemId, siCount);
	}

	return nullptr;
}

// There is a double copy but I am feeling evil today!
void arrayAddItem(auto& items, auto itemInstance, uint8_t maxItems, uint8_t& itemCounter)
{
	if (itemCounter >= maxItems)
		LOG_CRITICAL("Tried to add item to player when it is full!");
	else
		items[itemCounter++] = itemInstance;
}

void ModularShipComponent::AddItem(const ItemID itemId)
{
	Item* itemPtr = GetItem(itemId);
	if (itemPtr != nullptr)
	{
		itemPtr->LevelUp();
	}
	else
	{
		switch (itemId.category)
		{
		case ItemCategory::BaseStat:
			arrayAddItem(bsi, GR::gr->itemRegistry.CreateInstance<BaseStatItem>(itemId), bsiMax, bsiCount);
			break;
		case ItemCategory::ShipModule:
			arrayAddItem(smi, GR::gr->itemRegistry.CreateInstance<ShipModuleItem>(itemId), smiMax, smiCount);
			break;
		case ItemCategory::Unique:
			arrayAddItem(si, GR::gr->itemRegistry.CreateInstance<UniqueItem>(itemId), siMax, siCount);
			break;
		}
	}
}
