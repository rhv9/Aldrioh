#include <pch.h>
#include "EntityType.h"

uint16_t EntityType::counter = 0;
std::array<EntityType*, 48> EntityType::globalEntityTypes;

EntityType::EntityType(EntityCategory category, const std::string& name) : entityId(counter++, category)
{
	if (entityId.id >= globalEntityTypes.size())
		LOG_CORE_CRITICAL("Need to increase max number of entity types that can be referenced!");
	globalEntityTypes[entityId.id] = this;
}

const EntityType* EntityType::GetEntityType(entitytypeid_t)
{
	return nullptr;
}
