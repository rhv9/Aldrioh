#include <pch.h>
#define GAMEENTITIES_CPP
#include "GameEntities.h"

#include <Scene/Scene.h>

#include <Scene/Components.h>
#include <Game/Components/EntityComponents.h>
#include <Game/Components/ControllerComponents.h>

#include <Components/Path/PathComponent.h>
#include <Collision/Collision.h>
#include <Game/Components/LevelComponents.h>

#include <Scene/EntityCameraController.h>
#include <FreeCameraController.h>

#include <Game/SpriteCollection.h>
#include <Game.h>
#include <Game/GlobalLayers.h>

#include <Math/Math.h>

#include <Game/Level/CollectableChunk.h>


//======================
// Entity factories
//======================
static Entity drone_create(EntityType& type, Level& level, const glm::vec2& pos, int lvl, void* data)
{
	static ParticleTemplate particleTemplate_droneDestroyed = []() {
		ParticleTemplate pt;
		pt.beginColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pt.endColour = glm::vec4(1.0f, 1.0f, 1.0f, 0.7f);
		pt.beginSize = 0.25f;
		pt.endSize = 0.25f;
		pt.life = 1.1f;
		pt.velocity = { 0.0f, 0.0f };
		pt.velocityVariation = { 2.0f, 2.0f };
		pt.rotationRange = { Math::degreesToRad(-100), Math::degreesToRad(100) };
		pt.count = 7;
		return pt;
		}();

	static ParticleTemplate particleTemplate_droneDestroyedRed = []() {
		ParticleTemplate pt = particleTemplate_droneDestroyed;
		pt.beginColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		pt.endColour = glm::vec4(0.4f, 0.0f, 0.0f, 1.0f);
		pt.count = 1;
		return pt;
		}();

	static auto OnDestroy_DroneDeath = [](Entity e) -> void {
		if (e.GetComponent<HealthComponent>().health <= 0.0f)
		{
			glm::vec2 pos = e.GetTransformComponent().position;
			ParticleTemplate pt = particleTemplate_droneDestroyed;
			pt.startPos = pos;
			e.getScene()->GetParticleManager().Emit(pt);

			pt = particleTemplate_droneDestroyedRed;
			pt.startPos = pos;
			e.getScene()->GetParticleManager().Emit(pt);

			Level* level = e.getScene()->GetFirstEntity<LevelComponent>().GetComponent<LevelComponent>().level;
			EntityID entityId = e.GetComponent<EntityTypeComponent>().typeId;

			// Add item
			CollectableType collectableType = EnemyEntityTypes::GetEnemyEntityType(entityId.id)->collectableDrop;
			level->GetCollectableManager().AddCollectable(pos, collectableType);

			level->GetLevelStats().onEnemyKilled(entityId);
		}
		};

	Scene& scene = level.GetScene();
	EnemyEntityType& typeCasted = *((EnemyEntityType*)&type);

	Entity enemy = scene.CreateEntity("Drone");
	auto& tc = enemy.GetComponent<TransformComponent>();
	tc.UpdateBothPos(pos);
	VisualComponent& vc = enemy.AddComponent<VisualComponent>(typeCasted.spriteId);
	vc.localTransform = { -0.5f, -0.5f, 0.0f };
	if (type.entityId == EnemyEntityTypes::Drone_Colourful->entityId)
	{
		vc.colour = Colour::Random();
	}
	enemy.AddComponent<MoveControllerComponent>(typeCasted.speed);
	enemy.AddComponent<PhysicsMovementComponent>();
	enemy.AddComponent<EntityTypeComponent>(type.entityId);
	glm::vec2 collisionSize{ 0.5f };
	enemy.AddComponent<CollisionComponent>(glm::vec3{ collisionSize / -2.0f, 0.0f }, collisionSize, true);
	enemy.AddComponent<HealthComponent>(typeCasted.maxHp);
	enemy.AddComponent<CoreEnemyStateComponent>();
	enemy.AddComponent<FollowPlayerAIComponent>();
	enemy.AddComponent<OnDestroyComponent>(OnDestroy_DroneDeath);

	return enemy;
}


static Entity item_create(EntityType& type, Level& level, const glm::vec2& pos, int lvl, void* data)
{
	CollectableItem::RenderData itemRenderData = *((CollectableItem::RenderData*)data);
	Entity itemEntity = level.GetScene().CreateEntity("FlyingItem");
	VisualComponent& vc = itemEntity.AddComponent<VisualComponent>(itemRenderData.spriteId);
	vc.scale = itemRenderData.size;
	vc.localTransform = { -(vc.scale / 2.0f), 0.0f };
	vc.colour = itemRenderData.colour;
	glm::vec2 p0 = pos;
	itemEntity.GetTransformComponent().UpdateBothPos(p0);
	BezierPathComponent& bezier = itemEntity.AddComponent<BezierPathComponent>(p0, p0 + glm::vec2{ 0.0f, 1.5f }, level.GetPlayer().GetTransformComponent().position);
	bezier.onCompletionCallback = [](Entity e) {
		e.QueueDestroy();
		GlobalLayers::game->GetCurrentLevel()->GetPlayerStats().AddExp(5);
		};
	itemEntity.AddComponent<ItemAnimationControllerComponent>();
	return itemEntity;
}



//======================
// All enemy list
//======================


void EnemyInitGlobal();

void EntityTypes::InitGlobal()
{
	Player = new EntityType{ EntityCategory::Player, "Player" };
	Fireball = new EntityType{ EntityCategory::Bullet, "Fireball" };

	FlyingCollectedItem = new EntityType{ EntityCategory::FlyingItem, "Flying_Collected_Item" };
	FlyingCollectedItem->onCreateCallback = item_create;
	EnemyInitGlobal();
}

void EnemyInitGlobal()
{
	using namespace EnemyEntityTypes;

	Asteroid = new EnemyEntityType{ EntityCategory::Enemy, "Asteroid" };
	Asteroid->maxHp = 1.0f;
	Asteroid->speed = 0.5f;
	Asteroid->collectableDrop = CollectableType::NONE;

	EnemyEntityTypes::Enemy = new EnemyEntityType{ EntityCategory::Enemy, "Enemy" };
	EnemyEntityTypes::Enemy->maxHp = 1.0f;
	EnemyEntityTypes::Enemy->speed = 0.6f;
	EnemyEntityTypes::Enemy->collectableDrop = CollectableType::JEWEL1;

	Drone_Normal = new EnemyEntityType{ EntityCategory::Enemy, "Drone_Normal" };
	Drone_Normal->maxHp = 1.0f;
	Drone_Normal->speed = 2.25f;
	Drone_Normal->collectableDrop = CollectableType::JEWEL1;
	Drone_Normal->spriteId = Sprites::drone_normal;
	Drone_Normal->onCreateCallback = drone_create;

	Drone_Tank = new EnemyEntityType{ EntityCategory::Enemy, "Drone_Tank" };
	Drone_Tank->maxHp = 10.0f;
	Drone_Tank->speed = 1.0f;
	Drone_Tank->collectableDrop = CollectableType::JEWEL2;
	Drone_Tank->spriteId = Sprites::drone_tank;
	Drone_Tank->onCreateCallback = drone_create;

	Drone_Colourful = new EnemyEntityType{ EntityCategory::Enemy, "Drone_Colourful" };
	Drone_Colourful->maxHp = 0.1f;
	Drone_Colourful->speed = 3.0f;
	Drone_Colourful->collectableDrop = CollectableType::JEWEL1;
	Drone_Colourful->spriteId = Sprites::drone_normal;
	Drone_Colourful->onCreateCallback = drone_create;
}

EnemyEntityType* EnemyEntityTypes::GetEnemyEntityType(entitytypeid_t id)
{
	return (EnemyEntityType*)(EntityType::GetEntityType(id));
}

EnemyEntityType::EnemyEntityType(EntityCategory category, const std::string& name) : EntityType(category, name)
{
	static auto default_create = [](EntityType&, Level&, const glm::vec2&, int, void*) -> Entity
		{
			ASSERT(false, "No creation method has been placed!")
				return Entity::Null;
		};
	onCreateCallback = default_create;
}
