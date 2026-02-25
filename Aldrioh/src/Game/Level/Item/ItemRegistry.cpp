#include <pch.h>
#include "ItemRegistry.h"

void ItemRegistry::Debug_PrintRegistry()
{
	LOG_INFO("--ItemRegistry--");
	for (auto& [key, val] : itemDefMap)
		LOG_INFO("{}:{}", val.id.to_string(), val.name);
	LOG_INFO("----------------");
}
