#pragma once
#include "ItemTypes.h"

class ItemRegistry;
class Entity;

class FireBallShipModuleItem : public ShipModuleItem
{
public:
	FireBallShipModuleItem() = default;
	FireBallShipModuleItem(const ItemDef& def) : ShipModuleItem(def) {}
	virtual ~FireBallShipModuleItem() override {}
	virtual std::unique_ptr<Item> CreateCopy() { return std::make_unique<FireBallShipModuleItem>(*this); }

	virtual LvlUpInfo LevelUp();
	virtual void OnUpdate(Timestep ts, Entity e) override;
	virtual LvlUpInfo LevelUpPretend() override { FireBallShipModuleItem copyItem = *this; return copyItem.LevelUp(); }
	virtual void RecalculateOnStatChanges(StatModifier& statModifier) override;

	float shootTimer = 0.0f;
	float shootCooldown = 0.42f;

	float projectileCount = 1;
	float speed = 10.0f;

	float cachedDmg = 10;

	float dmg = 10;
	float dmg_mult = 0;
};


class MachineGunShipModuleItem : public ShipModuleItem
{
public:
	MachineGunShipModuleItem() = default;
	MachineGunShipModuleItem(const ItemDef& def) : ShipModuleItem(def) {}
	virtual ~MachineGunShipModuleItem() override {}
	virtual std::unique_ptr<Item> CreateCopy() { return std::make_unique<MachineGunShipModuleItem>(*this); }

	virtual LvlUpInfo LevelUp();
	virtual void OnUpdate(Timestep ts, Entity e) override;
	virtual LvlUpInfo LevelUpPretend() override { MachineGunShipModuleItem copyItem = *this; return copyItem.LevelUp(); }
	virtual void RecalculateOnStatChanges(StatModifier& statModifier) override;

	float shootTimer = 0.0f;
	float shootCooldown = 0.12f;

	float projectileCount = 1;
	float speed = 7.0f;

	float dmg = 2;
	float dmg_mult = 0;

	float cachedDmg = dmg;
};