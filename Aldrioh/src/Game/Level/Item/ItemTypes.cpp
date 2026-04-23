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

#include "ShipModuleItems.h"


void ItemTypes::Init(ItemRegistry& itemRegistry)
{
	auto& baseStat_Hp = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Hp, "HP Gear", "Increases your HP by a whole freaking lot boiii!", Sprites::item_health });
	baseStat_Hp.statModifier.hp_multiplier = 0.1f;
	baseStat_Hp.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.hp_multiplier;
			item->statModifier.hp_multiplier += 0.1f;
			return { "Increase HP by 10%" };
		};

	auto& baseStat_dmg = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Dmg, "Damage Gear", "Increases your damage", Sprites::item_dmg_increase });
	baseStat_dmg.statModifier.dmg_multiplier = 0.1f;
	baseStat_dmg.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.dmg_multiplier;
			item->statModifier.dmg_multiplier += 0.1f;
			return { "Increase damage by 10%" };
		};

	auto& baseStat_critChance = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_CritChance, "Crit Chance Gear", "Increases your chance of a critical hit", Sprites::item_crit_chance });
	baseStat_critChance.statModifier.critChance_multiplier = 0.1f;
	baseStat_critChance.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.critChance_multiplier;
			item->statModifier.critChance_multiplier += 0.1f;
			return { "Increase critical chance by 10%" };
		};

	auto& baseStat_critDamage = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_CritDamage, "Crit Damage Gear", "Increases your critical hit damage", Sprites::item_crit_dmg });
	baseStat_critDamage.statModifier.critDmg_multiplier = 0.1f;
	baseStat_critDamage.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.critDmg_multiplier;
			item->statModifier.critDmg_multiplier += 0.1f;
			return { "Increase critical damage by 10%" };
		};

	auto& baseStat_luck = itemRegistry.AddNewItem<BaseStatItem>(ItemDef{ ItemTypes::BaseStat_Luck, "Clover Gear", "Increases your luck", Sprites::item_clover });
	baseStat_luck.statModifier.luck_modifier = 0.1f;
	baseStat_luck.levelUpFunc = [](Item* itemPtr) -> LvlUpInfo
		{
			BaseStatItem* item = static_cast<BaseStatItem*>(itemPtr);
			++item->lvl;
			int old = item->statModifier.luck_modifier;
			item->statModifier.luck_modifier += 0.1f;
			return { "Increase your luck by 10%" };
		};


	// Ship modules!!

	auto& shipModule_shooter = itemRegistry.AddNewItem<FireBallShipModuleItem>(ItemDef{ ItemTypes::ShipModule_Shooter, "Fireball Shooter", "High fire rate weaponary", Sprites::item_fireball });


	// Unique items

	auto& unique_floatyFast = itemRegistry.AddNewItem<UniqueItem>(ItemDef{ ItemTypes::Unique_FloatyFast, "Floaty Fast", "Move very fast but something is fishy with gravity here", Sprites::item_floatyfast });
	unique_floatyFast.applyEffectsToPlayerFunc = [](Entity e)
		{
			auto& mcc = e.GetComponent<MoveControllerComponent>();
			mcc.maxSpeed *= 3.0f;
			mcc.speed *= 10.0f;
			mcc.falloffMultiplier *= 0.5f;
		};
	unique_floatyFast.updateFunc = [](Timestep ts, Entity e)
		{
		};

	itemRegistry.Debug_PrintRegistry();
}
