#if !defined(SPRITECOLLECTION_GUARD) || defined(SPRITES_CPP)
#define SPRITECOLLECTION_GUARD

#if defined(SPRITES_DECLARATION) && !defined(SPRITES_CPP)
#define SPRITES_EXTERN
#else
#define SPRITES_EXTERN extern
#endif

// In the cpp file, this header file is included twice with different defines, so that we have both declaration of global variable and initialisation of it.
#ifdef SPRITES_CPP
#define SPRITES_MACRO(name, x, y) toInitSprites.push_back(SpriteDataInternal{&name, glm::vec2{x, y}});
#define SPRITES_MACRO_SIZE(name, x, y, size) toInitSprites.push_back(SpriteDataInternal{&name, glm::vec2{x, y}, size});
#elif defined(SPRITES_DECLARATION)
#define SPRITES_MACRO(name, x, y) spriteid_t name = -1;
#define SPRITES_MACRO_SIZE(name, x, y, size) spriteid_t name = -1;
#else
#define SPRITES_MACRO(name, x, y) extern spriteid_t name;
#define SPRITES_MACRO_SIZE(name, x, y, size) extern spriteid_t name;
#endif

#ifdef SPRITES_CPP
#define SPRITES_ANIMATED_MACRO(name, x, y, size) toInitAnimSprites.push_back(AnimatedSpriteDataInternal {&name, glm::vec2{x, y}, size})
#elif defined(SPRITES_DECLARATION)
#define SPRITES_ANIMATED_MACRO(name, x, y, size) AnimatedSprite name {-1, 0}
#else
#define SPRITES_ANIMATED_MACRO(name, x, y, size) extern AnimatedSprite name
#endif

using spriteid_t = int;

#if defined(SPRITECOLLECTION_GUARD) && !defined(SPRITES_CPP) || !defined(SPRITECOLLECTION_GUARD) && !defined(SPRITES_CPP)
struct AnimatedSprite
{
	spriteid_t firstFrame;
	uint8_t frameCount;

	uint8_t GetFrameCount() const { return frameCount; }
	// parameter frame assumed to be between 0 and frameCount
	spriteid_t GetFrameUnsafe(uint8_t frame) { return firstFrame + frame; }
};
#endif

class Texture;
class SubTexture;

namespace Sprites {

#ifndef SPRITES_CPP
	void Init();
	constexpr glm::vec2 TileSize{ 16.0f, 16.0f };
	SubTexture* get(spriteid_t index);
#endif


// This gets defined once in SpriteCollection.cpp so that you can run the function to create the sprites.
#ifdef SPRITES_CPP
	struct SpriteDataInternal
	{
		spriteid_t* spriteLocation;
		glm::vec2 pos;
		glm::vec2 size{ 1.0f, 1.0f };
	};

	struct AnimatedSpriteDataInternal
	{
		AnimatedSprite* animSpriteLocation;
		glm::vec2 pos;
		int frameCount;
	};

	void RunMonsterCodeThatInitialisesSpritesFromHeaderFileUsingVeryReadableMacros() {
		std::vector<SpriteDataInternal> toInitSprites;
		std::vector<AnimatedSpriteDataInternal> toInitAnimSprites;

#endif

		// Subtextures
		SPRITES_MACRO(null, 1, 1);
		SPRITES_MACRO(shadow, 0, 3);
		SPRITES_MACRO(sand_1, 0, 0);
		SPRITES_MACRO(sand_rock, 1, 0);
		SPRITES_MACRO(sand_cactus, 2, 0);
		SPRITES_MACRO(bullet_fire, 0, 1);
		SPRITES_MACRO(bullet_red, 1, 1);
		SPRITES_MACRO(bullet_white, 2, 1);
		SPRITES_MACRO(bullet_rocket, 5, 1);

		SPRITES_MACRO(particle_smoke_1, 6, 1);
		SPRITES_MACRO(particle_smoke_2, 7, 1);
		SPRITES_MACRO(particle_drone_broken_1, 8, 1);
		SPRITES_MACRO(particle_drone_broken_2, 9, 1);
		SPRITES_MACRO(particle_drone_broken_red, 10, 1);

		SPRITES_MACRO(coin, 3, 1);
		SPRITES_MACRO(jewel, 4, 1);

		SPRITES_MACRO(player_head, 0, 2);
		SPRITES_MACRO(slime, 1, 2);
		SPRITES_MACRO(target, 2, 2);
		SPRITES_MACRO(spawner, 3, 2);

		SPRITES_MACRO(player_ship, 0, 15);

		SPRITES_MACRO(asteroid_small, 0, 14);
		SPRITES_MACRO(drone_normal, 0, 13);
		SPRITES_MACRO(drone_tank, 1, 13);
		SPRITES_MACRO_SIZE(two_wing, 0, 10, glm::vec2{ 2 });
		SPRITES_MACRO_SIZE(diamond_enemy, 2, 10, glm::vec2{ 2 });

		SPRITES_MACRO(borderBox, 0, 0);
		SPRITES_MACRO(greenBox, 1, 0);
		SPRITES_MACRO(square, 2, 0);

		// Items
		SPRITES_MACRO(item_fireball,		15, 15);
		SPRITES_MACRO(item_health,			15, 14);
		SPRITES_MACRO(item_floatyfast,		15, 13);
		SPRITES_MACRO(item_dmg_increase,	15, 12);
		SPRITES_MACRO(item_crit_dmg,		15, 11);
		SPRITES_MACRO(item_crit_chance,		15, 10);
		SPRITES_MACRO(item_clover,			15,	 9);
		SPRITES_MACRO(item_machinegun,		15,  8);
		SPRITES_MACRO(item_rocketshooter,	15,  7);

		SPRITES_ANIMATED_MACRO(anim_energycore_drone, 0, 12, 4);


// It iterates through the list so that we can add it in.
#ifdef SPRITES_CPP
		LOG_CORE_INFO("Number of sprites using new system:{}", toInitSprites.size());
		
		for (SpriteDataInternal& spi : toInitSprites)
		{
			spriteid_t* spriteLoc = spi.spriteLocation;
			*spriteLoc = spriteCounter++;
			spriteMap[*spriteLoc] = { spritesheet, spi.pos, Sprites::TileSize, spi.size };
		}

		LOG_CORE_INFO("Number of animated sprites using new system:{}", toInitAnimSprites.size());
		
		for (AnimatedSpriteDataInternal& aspi : toInitAnimSprites)
		{
			AnimatedSprite* animSpriteLoc = aspi.animSpriteLocation;
			animSpriteLoc->firstFrame = spriteCounter;
			animSpriteLoc->frameCount = aspi.frameCount;
			for (int i = 0; i < aspi.frameCount; ++i)
			{
				spriteMap[spriteCounter++] = { spritesheet, {aspi.pos.x + i, aspi.pos.y}, Sprites::TileSize};
			}
		}

		LOG_CORE_INFO("Total loaded sprites:{}", spriteCounter);
	}
#endif

	SPRITES_EXTERN std::vector<int> animPlayerUp, animPlayerDown, animPlayerLeft, animPlayerRight;
	SPRITES_EXTERN std::vector<int> animBossUp, animBossDown, animBossLeft, animBossRight;
}
#endif
