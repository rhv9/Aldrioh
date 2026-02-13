#pragma once
#include "EntityType.h"
#include <Game/Level/CollectableChunk.h>
#include <Scene/Entity.h>
#include <Game/SpriteCollection.h>

#ifdef GAMEENTITIES_CPP
#define GE_GLOBAL_VARIABLE(VAR_NAME) VAR_NAME = nullptr
#define EXTERN_GAMEENTITIES
#else
#define GE_GLOBAL_VARIABLE(VAR_NAME) VAR_NAME
#define EXTERN_GAMEENTITIES extern
#endif

class Level;

struct EnemyEntityType : public EntityType
{
	float maxHp = 1.0f;
	float speed = 1.0f;
	CollectableType collectableDrop = CollectableType::NONE;
	float dmg = 1.0f;
	spriteid_t spriteId = 0;

	std::function<Entity(EnemyEntityType& entityType, Scene& scene, const glm::vec2& pos, int lvl)> createFunc;
	void OnPostCreate(Level& level, Entity e);

	EnemyEntityType(EntityCategory category, const std::string& name);
	Entity create(Scene& scene, Level& level, const glm::vec2& pos, int lvl) { Entity e = createFunc(*this, scene, pos, lvl); OnPostCreate(level, e); return e; }
};

namespace EntityTypes
{
	EXTERN_GAMEENTITIES EntityType* GE_GLOBAL_VARIABLE(Player);
	EXTERN_GAMEENTITIES EntityType* GE_GLOBAL_VARIABLE(Fireball);
	void InitGlobal();
}


namespace EnemyEntityTypes {

	EnemyEntityType* GetEnemyEntityType(entitytypeid_t id);

	// Enemy types
	EXTERN_GAMEENTITIES EnemyEntityType* GE_GLOBAL_VARIABLE(Asteroid);
	EXTERN_GAMEENTITIES EnemyEntityType* GE_GLOBAL_VARIABLE(Enemy);
	EXTERN_GAMEENTITIES EnemyEntityType* GE_GLOBAL_VARIABLE(Drone_Normal);
	EXTERN_GAMEENTITIES EnemyEntityType* GE_GLOBAL_VARIABLE(Drone_Tank);
	EXTERN_GAMEENTITIES EnemyEntityType* GE_GLOBAL_VARIABLE(Drone_Colourful);
}