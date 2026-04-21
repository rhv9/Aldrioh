#include <pch.h>
#include "EntityComponents.h"

#include <Game/GlobalResource.h>

Item* arrayGetItem(auto& items, const auto elem, auto itemCount)
{
	for (int i = 0; i < itemCount; ++i)
		if (items[i]->id == elem)
			return items[i].get();
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
		return arrayGetItem(baseStatItems, itemId, baseStatCount);
	case ItemCategory::ShipModule:
		return arrayGetItem(shipModuleItems, itemId, shipModuleCount);
	case ItemCategory::Unique:
		return arrayGetItem(uniqueItems, itemId, uniqueCount);
	}

	return nullptr;
}

// There is a double copy but I am feeling evil today!
void arrayAddItem(auto& items, auto itemInstance, uint8_t maxItems, uint8_t& itemCounter)
{
	if (itemCounter >= maxItems)
		LOG_CRITICAL("Tried to add item to player when it is full!");
	else
		items[itemCounter++] = std::move(itemInstance);
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
			arrayAddItem(baseStatItems, std::move(GR::gr->itemRegistry.CreateInstance(itemId)), baseStatMax, baseStatCount);
			break;
		case ItemCategory::ShipModule:
			arrayAddItem(shipModuleItems, std::move(GR::gr->itemRegistry.CreateInstance(itemId)), shipModuleMax, shipModuleCount);
			break;
		case ItemCategory::Unique:
			arrayAddItem(uniqueItems, std::move(GR::gr->itemRegistry.CreateInstance(itemId)), uniqueMax, uniqueCount);
			break;
		}
	}
}

StatModifier ModularShipComponent::CalculateTotalStatModifier() const
{
	StatModifier total;
	for (int i = 0; i < baseStatCount; ++i)
		total += static_cast<BaseStatItem*>(baseStatItems[i].get())->statModifier;
	return total;
}
