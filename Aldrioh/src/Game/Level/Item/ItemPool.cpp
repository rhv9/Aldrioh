#include <pch.h>
#include "ItemPool.h"

#include "ItemRegistry.h"
#include <Game/GlobalResource.h>

#include <Math/Math.h>

ItemPool::ItemPool()
{
	itemPool = {
		ItemTypes::BaseStat_Hp,
		ItemTypes::BaseStat_Dmg,
		ItemTypes::BaseStat_CritChance,
		ItemTypes::BaseStat_CritDamage,
		ItemTypes::BaseStat_Luck,

		ItemTypes::ShipModule_Shooter,
		ItemTypes::ShipModule_MachineGun,
		ItemTypes::ShipModule_RocketShooter,
		//ItemTypes::ShipModule_AOEDrone,
		//
		ItemTypes::Unique_FloatyFast,
	};
}

std::array<ItemID, 3> ItemPool::GenerateThreeUniqueItems()
{
	std::array<int, 3> items;

	uint8_t counter = 0;
	while (counter < items.size())
	{
		int pos = Math::Random::linearInt(0, itemPool.size() - 1);

		bool newPos = true;
		for (int i = 0; i < counter; ++i)
			if (items[i] == pos)
				newPos = false;

		if (newPos)
			items[counter++] = pos;
	}

	return { itemPool[items[0]], itemPool[items[1]], itemPool[items[2]] };
}
