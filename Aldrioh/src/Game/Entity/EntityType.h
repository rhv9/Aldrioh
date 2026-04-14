#pragma once
#include <Game/Level/CollectableChunk.h>

class Level;

using entitycategorytype_t = uint8_t;
using entitytypeid_t = uint16_t;

enum EntityCategory : entitycategorytype_t
{
	Player = 0,
	Enemy,
	DamageBox,
	Bullet,
	FlyingItem,
};


struct EntityID
{
	entitytypeid_t id;
	EntityCategory category;

	bool operator==(const EntityID& rhs) const { return id == rhs.id; }
	bool IsSameCategory(EntityID type) const { return category == type.category; }
};

struct EntityType
{
	EntityID entityId;
	std::string name;

	EntityType(EntityCategory category, const std::string& name);

	bool operator==(const EntityType& rhs) const { return entityId.id == rhs.entityId.id; }
	bool IsSameCategory(EntityType type) const { return entityId.category == type.entityId.category; }

	std::function<Entity(EntityType& entityType, Level& level, const glm::vec2& pos, int lvl, void* dataPtr)> onCreateCallback;
	void OnPostCreate(Level& level, Entity e);

	Entity create(Level& level, const glm::vec2& pos, int lvl, void* dataPtr);

	static const EntityType* GetEntityType(entitytypeid_t);
private:
	static uint16_t counter;
	static std::array<EntityType*, 48> globalEntityTypes; // Globally set because easier
};


