#include <pch.h>
#define GAMEENTITIES_CPP
#include "GameEntities.h"

void EnemyInitGlobal();

void EntityTypes::InitGlobal()
{
	Player = new EntityType{ EntityCategory::Player, "Player" };
	Fireball = new EntityType{ EntityCategory::Bullet, "Fireball" };

	EnemyInitGlobal();
}

void EnemyInitGlobal()
{
	using namespace EnemyEntityTypes;

	Asteroid = new EnemyEntityType{ EntityCategory::Enemy, "Asteroid" };
	Asteroid->maxHp = 1.0f;
	Asteroid->collectableDrop = CollectableType::NONE;

	EnemyEntityTypes::Enemy = new EnemyEntityType{ EntityCategory::Enemy, "Enemy" };
	EnemyEntityTypes::Enemy->maxHp = 1.0f;
	EnemyEntityTypes::Enemy->collectableDrop = CollectableType::JEWEL1;

	Drone_Normal = new EnemyEntityType{ EntityCategory::Enemy, "Drone_Normal" };
	Drone_Normal->maxHp = 1.0f;
	Drone_Normal->collectableDrop = CollectableType::JEWEL1;

	Drone_Tank = new EnemyEntityType{ EntityCategory::Enemy, "Drone_Tank" };
	Drone_Tank->maxHp = 1.0f;
	Drone_Tank->collectableDrop = CollectableType::JEWEL1;

	Drone_Colourful = new EnemyEntityType{ EntityCategory::Enemy, "Drone_Colourful" };
	Drone_Colourful->maxHp = 1.0f;
	Drone_Colourful->collectableDrop = CollectableType::JEWEL1;

}

EnemyEntityType* EnemyEntityTypes::GetEnemyEntityType(entitytypeid_t id)
{
	return (EnemyEntityType*)(EntityType::GetEntityType(id));
}
