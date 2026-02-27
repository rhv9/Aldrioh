#include <pch.h>
#include "ItemRegistry.h"

ItemDef& ItemRegistry::GetItemDef(ItemID itemId)
{
	static ItemDef failedDef{ 0, ItemCategory::Count };

	if (itemDefMap.find(itemId) == itemDefMap.end())
	{
		LOG_CRITICAL("ItemRegistry::GetItemDef does not have {}", itemId.to_string());
		return failedDef;
	}
	return itemDefMap[itemId];
}

void ItemRegistry::Debug_PrintRegistry()
{
	LOG_INFO("--ItemRegistry--");
	for (auto& [key, val] : itemDefMap)
		LOG_INFO("{}:{}", val.id.to_string(), val.name);
	LOG_INFO("----------------");
}
