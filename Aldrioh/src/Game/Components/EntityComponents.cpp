#include <pch.h>
#include "EntityComponents.h"

Item* arrayGetItem(auto items, auto elem, auto itemCount)
{
    for (int i = 0; i < itemCount; ++i)
        if (items[i].id == elem)
            return &items[i];

    return nullptr;
}

bool ModularShipComponent::HasItem(ItemID itemId)
{
    switch (itemId.category)
    {
    case ItemCategory::BaseStat:
        return arrayGetItem(bsi, itemId, bsiCount) != nullptr;
    case ItemCategory::ShipModule:
        return arrayGetItem(smi, itemId, smiCount) != nullptr;
    case ItemCategory::Unique:
        return arrayGetItem(si, itemId, siCount) != nullptr;
    }
    return false;
}

Item* ModularShipComponent::GetItem(ItemID itemId)
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
