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

	float shootTimer = 0.0f;
	float projectileCount = 1;
};