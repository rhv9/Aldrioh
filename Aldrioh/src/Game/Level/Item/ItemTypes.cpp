#include <pch.h>
#include "ItemTypes.h"
#include "ItemRegistry.h"
#include <Game/SpriteCollection.h>

#include <Scene/Entity.h>
#include <Game/Components/EntityComponents.h>

#include <Math/Math.h>
#include <Collision/Collision.h>
#include <Game/Entity/EntityType.h>
#include <Game/Level/CollectableManager.h>
#include <Game/Components/LevelComponents.h>

#include <Scene/Components.h>

void shootBall(Entity& e, const glm::vec2& origin, const glm::vec2& normalizedDir)
{
	// Create entity
	Entity fireball = e.getScene()->CreateEntity("Fireball");
	fireball.GetComponent<TransformComponent>().position = glm::vec3{ origin , 0.5f };
	auto& mc = fireball.AddComponent<MoveComponent>(20.0f);
	mc.addMoveVec(normalizedDir);
	mc.locked = true;
	VisualComponent& vc = fireball.AddComponent<VisualComponent>(Sprites::bullet_fire, glm::vec3{ -0.5f, -0.5f, 0.0f });
	vc.rotation = Math::angle(mc.moveVec);
	vc.colour.a = 1.0f;
	fireball.AddComponent<TimeLifeComponent>(1.0f);
	fireball.AddComponent<EntityTypeComponent>(EntityTypes::Fireball->entityId);
	glm::vec2 collisionSize{ 0.3f };
	fireball.AddComponent<CollisionComponent>(glm::vec3{ collisionSize / -2.0f, 0.0f }, collisionSize);
}


void ItemTypes::Init(ItemRegistry& itemRegistry)
{
	auto& baseStat_Hp = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Hp, "HP Gear", "Increases your HP by a whole freaking lot boiii!", Sprites::asteroid_small });
	baseStat_Hp.statModifier.hp_multiplier = 10;
	baseStat_Hp.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.hp_multiplier;
			item->statModifier.hp_multiplier += 10;
			return { "Increase HP by 10%" };
		};

	auto& baseStat_dmg = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Dmg, "Damage Gear", "Increases your damage", Sprites::asteroid_small });
	baseStat_dmg.statModifier.dmg_multiplier = 10;
	baseStat_dmg.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.dmg_multiplier;
			item->statModifier.dmg_multiplier += 10;
			return { "Increase damage by 10%" };
		};

	auto& baseStat_critChance = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_CritChance, "Crit Chance Gear", "Increases your chance of a critical hit", Sprites::asteroid_small });
	baseStat_critChance.statModifier.critChance_multiplier = 10;
	baseStat_critChance.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.critChance_multiplier;
			item->statModifier.critChance_multiplier += 10;
			return { "Increase critical chance by 10%" };
		};

	auto& baseStat_critDamage = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_CritDamage, "Crit Damage Gear", "Increases your critical hit damage", Sprites::asteroid_small });
	baseStat_critDamage.statModifier.critDmg_multiplier = 10;
	baseStat_critDamage.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.critDmg_multiplier;
			item->statModifier.critDmg_multiplier += 10;
			return { "Increase critical damage by 10%" };
		};

	auto& baseStat_luck = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Luck, "Clover Gear", "Increases your luck", Sprites::asteroid_small });
	baseStat_luck.statModifier.luck_modifier = 10;
	baseStat_luck.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.luck_modifier;
			item->statModifier.luck_modifier += 10;
			return { "Increase your luck by 10%" };
		};


	// Ship modules!!

	struct ShipModuleShooterData
	{
		float shootTimer = 0.0f;
	};
	ShipModuleShooterData shipModuleShooterData;

	auto& shipModule_shooter = itemRegistry.AddNewItem<ShipModuleItem>(ItemDef{ ItemTypes::ShipModule_Shooter, "Fireball Shooter", "High fire rate weaponary", Sprites::asteroid_small });
	shipModule_shooter.updateFunc = [data = shipModuleShooterData](Timestep ts, Entity e) mutable
		{
			auto& inputAction = e.GetComponent<ActionComponent>();
			if (inputAction.shoot)
			{
				if (data.shootTimer >= 0.08f || data.shootTimer == 0.0f)
				{
					data.shootTimer = std::max(data.shootTimer - 1.0f, 0.0f);
					glm::vec3& playerPos = e.GetTransformComponent().position;
					shootBall(e, playerPos, inputAction.dir);
					e.getScene()->CreateEntity("Sound").AddComponent<SoundComponent>("player_shoot");

				}
				data.shootTimer += ts;
			}
			else
				data.shootTimer = 0.0f;
		};

	itemRegistry.Debug_PrintRegistry();
}
