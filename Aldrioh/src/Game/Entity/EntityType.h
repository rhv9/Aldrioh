#pragma once
#include <Game/Level/CollectableChunk.h>

enum EntityCategory : uint8_t
{
	Player = 0,
	Enemy,
	DamageBox,
	Bullet,
};

using entitytypeid_t = uint16_t;

struct EntityID
{
	entitytypeid_t id;
	EntityCategory category;

	bool operator==(const EntityID& rhs) const { return id == rhs.id; }
	bool IsSameCategory(EntityID type) const { return category == type.category; }
};

struct EntityType
{
	EntityType(EntityCategory category, const std::string& name);
	
	EntityID entityId;
	std::string name;

	bool operator==(const EntityType& rhs) const { return entityId.id == rhs.entityId.id; }
	bool IsSameCategory(EntityType type) const { return entityId.category == type.entityId.category; }

	static const EntityType* GetEntityType(entitytypeid_t);
private:
	static uint16_t counter;
	static std::array<EntityType*, 48> globalEntityTypes; // Globally set because easier
};


