
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
#elif defined(SPRITES_DECLARATION)
#define SPRITES_MACRO(name, x, y) spriteid_t name = -1;
#else
#define SPRITES_MACRO(name, x, y) extern spriteid_t name;
#endif

using spriteid_t = int;

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
		glm::vec2 size;
	};
	void RunMonsterCodeThatInitialisesSpritesFromHeaderFileUsingVeryReadableMacros() {
		std::vector<SpriteDataInternal> toInitSprites;
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

		SPRITES_MACRO(borderBox, 0, 0);
		SPRITES_MACRO(greenBox, 1, 0);
		SPRITES_MACRO(square, 2, 0);

// It iterates through the list so that we can add it in.
#ifdef SPRITES_CPP
		LOG_CORE_INFO("Number of sprites using new system:{}", toInitSprites.size());
		
		for (SpriteDataInternal& spi : toInitSprites)
		{
			spriteid_t* spriteLoc = spi.spriteLocation;
			glm::vec2 spriteSize = spi.size;
			*spriteLoc = spriteCounter++;
			spriteMap[*spriteLoc] = { spritesheet, spriteSize, Sprites::TileSize };
		}
	}
#endif

	SPRITES_EXTERN std::vector<int> animPlayerUp, animPlayerDown, animPlayerLeft, animPlayerRight;
	SPRITES_EXTERN std::vector<int> animBossUp, animBossDown, animBossLeft, animBossRight;
}
#endif
