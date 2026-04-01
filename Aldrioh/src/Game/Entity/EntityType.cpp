#include <pch.h>
#include "EntityType.h"
#include <Game/Level/Level.h>
#include <Scene/Entity.h>

uint16_t EntityType::counter = 0;
std::array<EntityType*, 48> EntityType::globalEntityTypes;

EntityType::EntityType(EntityCategory category, const std::string& name) : entityId(counter++, category), name(name)
{
	if (entityId.id >= globalEntityTypes.size())
		LOG_CORE_CRITICAL("Need to increase max number of entity types that can be referenced!");
	else
		globalEntityTypes[entityId.id] = this;
}

void EntityType::OnPostCreate(Level& level, Entity e)
{
	level.GetLevelStats().addEntityCount(entityId);
}

Entity EntityType::create(Level& level, const glm::vec2& pos, int lvl, void* dataPtr)
{
	Entity e = onCreateCallback(*this, level, pos, lvl, dataPtr); 
	OnPostCreate(level, e); 
	return e;
}

const EntityType* EntityType::GetEntityType(entitytypeid_t id)
{
	return globalEntityTypes[id];
}
