#pragma once

enum EntityCategory : uint8_t
{
	Player = 0,
	Enemy,
	DamageBox,
	Bullet,
};

struct EntityType
{
	uint16_t id;
	uint8_t category;
	EntityType(uint8_t category) : id(counter++), category(category) {}

	bool operator==(const EntityType& rhs) const { return id == rhs.id; }
	bool IsSameCategory(EntityType type) const { return category == type.category; }

private:
	static uint16_t counter;
};

namespace EntityTypes
{
	inline EntityType Player{ EntityCategory::Player };

	// Enemy types
	inline EntityType Asteroid { EntityCategory::Enemy };
	inline EntityType Enemy    { EntityCategory::Enemy };

	// Bullets
	inline EntityType Fireball { EntityCategory::Bullet };
}

